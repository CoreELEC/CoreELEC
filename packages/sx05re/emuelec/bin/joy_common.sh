#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)
# Copyright (C) 2022-present Joshua L (https://github.com/Langerz82)

# Source predefined functions and variables
. /etc/profile

GCDB="${SDL_GAMECONTROLLERCONFIG_FILE}"

EMULATOR="$1"

mkdir -p "/tmp/jc"

DEBUG_FILE="/tmp/jc/${EMULATOR}_joy_debug.cfg"
CACHE_FILE="/tmp/jc/${EMULATOR}_joy_cache.cfg"


jc_get_players() {
# You can set up to 8 player on ES
  declare -i PLAYER=1

# Dump gamepad information, gamepads NEED to be setup in gamecontrollerdb.txt which should already be done from Emulationstation
  gamepad_info -more > /tmp/input_devices 

# Determine how many gamepads/players are connected
  JOY_COUNT=$(gamepad_info | wc -l)

  declare -a PLAYER_CFGS=()
  for ((y = 1; y <= ${JOY_COUNT}; y++)); do

    declare -i JOY_INDEX=$(( y-1 ))
    local H_REGEX="instance id: ${JOY_INDEX}"
    local DEVICE_GUID=$(cat /tmp/input_devices | grep -E -A 3 "${H_REGEX}" | grep "guid:" | sed "s|\ \ \ \ \ \ \ \ guid: ||")

# if there is no GUID then most likely it has not been setup on Emulationstation (e.g there is no entry in gamecontrollerdb.txt)
    [[ -z "${DEVICE_GUID}" ]] && continue

    local JOY_NAME=$(jc_get_device_name "${DEVICE_GUID}")
    local JSI=$(jc_get_device_name "${DEVICE_GUID}" "true")

    # Add the joy config to array if guid and joyname set.
    if [[ ! -z "${DEVICE_GUID}" && ! -z "$JOY_NAME" ]]; then
      local PLAYER_CFG="${PLAYER} ${JSI} ${DEVICE_GUID} ${JOY_NAME}"

      PLAYER_CFGS+=("${PLAYER_CFG}")
      ((PLAYER++))
    fi
  done

  rm /tmp/input_devices

  if [[ -z "${PLAYER_CFGS[@]}" ]]; then
    echo "NO GAME CONTROLLERS WERE DETECTED."
    return
  fi

  SDL_CHECK=$(get_ee_setting ${EMULATOR}_joy_sdl_check)

  if [[ "${SDL_CHECK}" == "1" ]]; then
    jc_get_sdl_order

    declare -i PLAYER=1
    for SDL_NAME in "${SDL_GC_ORDER[@]}"; do
      declare -i index=0
      for PLAYER_CFG in "${PLAYER_CFGS[@]}"; do
        local JOY_NAME=$(echo $PLAYER_CFG | cut -d' ' -f4-)
        if [[ "$SDL_NAME" == "${JOY_NAME}" ]]; then
          PLAYER_CFG="${PLAYER} ${PLAYER_CFG:2}"
          jc_setup_gamecontroller "${PLAYER_CFG}"
          unset 'PLAYER_CFGS[index]'
        fi
        (( index++ ))
      done
      (( PLAYER++ ))
    done
  else
    for PLAYER_CFG in "${PLAYER_CFGS[@]}"; do
      jc_setup_gamecontroller "${PLAYER_CFG}"
    done
  fi
}

jc_setup_gamecontroller() {
  local PLAYER_CFG="$1"
  local JOY_INFO=$(echo "$1" | cut -d' ' -f1-3)
  local JOY_NAME=$(echo "$1" | cut -d' ' -f4-)

  [[ -z "${CACHE_FILE}" ]] && return

  local USE_CACHE=$(get_ee_setting ${EMULATOR}_joy_cache)
  if [[ "${USE_CACHE}" == "1" ]]; then
    local CACHE=""
    [[ -f "${CACHE_FILE}" ]] && CACHE=$(cat $CACHE_FILE | grep "$PLAYER_CFG")

    if [[ ! -z "$CACHE" || "$CACHE" == "${PLAYER_CFG}" ]]; then
      return
    fi
  fi

  clean_pad ${PLAYER_CFG}
  # echo "CONTROLLER: ${PLAYER_CFG}"
  set_pad ${JOY_INFO} "${JOY_NAME}"

  [[ -f "${CACHE_FILE}" ]] && sed -i "/^${PLAYER_CFG:0:1} js.*$/d" ${CACHE_FILE}
  echo "${PLAYER_CFG}" >> "${CACHE_FILE}"
}

declare SDL_GC_ORDER=()

jc_get_sdl_order() {
  echo "Performing sdljoytest"

  SDL_OUT="/tmp/sdljoytest.txt"

  sdljoytest > "${SDL_OUT}" &
  SDL_PID=$!
  sleep 2
  kill -9 $SDL_PID
  wait $SDL_PID 2>/dev/null

  while read line; do
    local REC=$(echo $line | grep "SDL_JOYDEVICEADDED")
    if [[ ! -z "$REC" ]]; then
      local JOY_NAME=$(echo $REC | cut -d ' ' -f4- )
      SDL_GC_ORDER+=("${JOY_NAME:1:-16}")
    fi
  done < "${SDL_OUT}"

  rm "${SDL_OUT}"
}

jc_get_device_name() {
  local GUID="${1}"
  local RETURN_JSI="${2}"

  v=${DEVICE_GUID:0:8}
  local p1=$(echo ${v:6:2}${v:4:2}${v:2:2}${v:0:2}) # Bus, generally not needed
  local v=${GUID:8:8}
  local p2=$(echo ${v:6:2}${v:4:2}${v:2:2}${v:0:2}) # Vendor
  v=${GUID:16:8}
  local p3=$(echo ${v:6:2}${v:4:2}${v:2:2}${v:0:2}) # Product
  v=${GUID:24:8}
  local p4=$(echo ${v:6:2}${v:4:2}${v:2:2}${v:0:2}) # Version

  local vendor=$(echo ${p2:4})
  local product=$(echo ${p3:4})
  local version=$(echo ${p4:4})

  local I_REGEX="^I\: .* Vendor\=${vendor} Product\=${product} Version\=${version}$"
  local EE_DEV=$(cat /proc/bus/input/devices | grep -Ew -A 8 "$I_REGEX" | grep -Ew -B 8 "B: KEY\=[0-9a-f ]+")
  local JSI=$(echo -e "${EE_DEV}" | grep "H: Handlers" | sed -E 's/.*H: Handlers=.*(js[0-9]).*/\1/')
  
  if [[ ! -z "${EE_DEV}" ]]; then
    local JOY_NAME=$(echo "${EE_DEV}" | grep -E "^N\: Name.*[\= ]?.*$" \
      | cut -d "=" -f 2 | tr -d '"')
    [ "${RETURN_JSI}" == "true" ] && echo ${JSI} || echo "${JOY_NAME}"
    return
  fi
  echo "0"
}
