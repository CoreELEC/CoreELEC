#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

GCDB="/storage/.config/SDL-GameControllerDB/gamecontrollerdb.txt"

EMULATOR="$1"

mkdir -p "/tmp/jc"

DEBUG_FILE="/tmp/jc/${EMULATOR}_joy_debug.cfg"
CACHE_FILE="/tmp/jc/${EMULATOR}_joy_cache.cfg"


jc_get_players() {
# You can set up to 8 player on ES
  declare -i PLAYER=1

  cat /proc/bus/input/devices | grep -E -B 5 -A 3 "^H\: Handlers=event[0-9] js[0-9].$" > /tmp/input_devices

  declare -a PLAYER_CFGS=()
  for ((y = 1; y <= 8; y++)); do
    #echo "Getting GUID for INPUT P${y}GUID" #debug

    local DEVICE_GUID=$(get_es_setting string "INPUT P${y}GUID")

    declare -i JOY_INDEX=$y-1
    local JSI="js${JOY_INDEX}"
    local JOY_NAME="0"

    # If the guid is set in ES then try and get the joyname from input_devices.
    if [[ ! -z "$DEVICE_GUID" ]]; then
      JOY_NAME=$(jc_get_device_name "$JSI" "$DEVICE_GUID")
    fi

    if [[ "$JOY_NAME" == "0" ]]; then
      local H_REGEX="^H: Handlers\=event[0-9] ${JSI}.*$"

      local GUID_LINE=$(cat /tmp/input_devices | grep -E -B 5 -A 3 "$H_REGEX" \
        | grep -E -B 8 "^B: KEY\=[0-9a-f ]+$" | grep -E -A 1 "^I: .*$")

      [[ -z "$GUID_LINE" ]] && continue

      # If guid isnt set then generate it from input_devices manually.
      [[ -z "$DEVICE_GUID" ]] && DEVICE_GUID=$(jc_generate_guid "$GUID_LINE")
      [[ -z "$DEVICE_GUID" ]] && continue

      JOY_NAME=$(echo "$GUID_LINE" | grep -E "^N: Name\=.*$" \
        | cut -d'=' -f 2 | tr -d '"')
    fi

    # If we cant get JOY_NAME from input_devices then try and retrieve it from gamecontrollerdb.txt
    if [[ "$JOY_NAME" == "0" ]]; then
      local GC_CONFIG=$(cat "$GCDB" | grep "$DEVICE_GUID" | grep "platform:Linux" | head -1)
      echo "GC_CONFIG=$GC_CONFIG"
      [[ -z $GC_CONFIG ]] && continue

      [[ -z "$JOY_NAME" ]] && JOY_NAME=$(echo $GC_CONFIG | cut -d',' -f2)
    fi

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
  SDL_CHECK=
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
  local JOY_P1=$(echo "$1" | cut -d' ' -f1-3)
  local JOY_P2=$(echo "$1" | cut -d' ' -f4-)

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
  echo "CONTROLLER: ${PLAYER_CFG}"
  set_pad ${JOY_P1} "${JOY_P2}"

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

jc_generate_guid() {
  local GUID_STRING="$1"

  local p1=$( echo $GUID_STRING | cut -d = -f2 | cut -d ' ' -f1 \
    | awk '{ printf "%8s\n", $0 }' | sed 's/ /0/g')
  local p2=$( echo $GUID_STRING | cut -d = -f3 | cut -d ' ' -f1 \
    | awk '{ printf "%8s\n", $0 }' | sed 's/ /0/g')
  local p3=$( echo $GUID_STRING | cut -d = -f4 | cut -d ' ' -f1 \
    | awk '{ printf "%8s\n", $0 }' | sed 's/ /0/g')
  local p4=$( echo $GUID_STRING | cut -d = -f5 | cut -d ' ' -f1 \
    | awk '{ printf "%8s\n", $0 }' | sed 's/ /0/g')

  local v=$(echo ${p1:6:2}${p1:4:2}${p1:2:2}${p1:0:2})
        v+=$(echo ${p2:6:2}${p2:4:2}${p2:2:2}${p2:0:2})
        v+=$(echo ${p3:6:2}${p3:4:2}${p3:2:2}${p3:0:2})
        v+=$(echo ${p4:6:2}${p4:4:2}${p4:2:2}${p4:0:2})

  echo "$v"
}

jc_get_device_name() {
  local JSI="$1"
  local GUID="$2"

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
  local EE_DEV=$(cat /proc/bus/input/devices | grep -Ew -A 8 "$I_REGEX" \
    | grep -E -B 5 -A 3 "^H: Handlers\=event[0-9] ${JSI}$" \
    | grep -Ew -B 8 "B: KEY\=[0-9a-f ]+" )

  if [[ ! -z "${EE_DEV}" ]]; then
    local JOY_NAME=$(echo "$EE_DEV" | grep -E "^N\: Name.*[\= ]?.*$" \
      | cut -d "=" -f 2 | tr -d '"')
    echo "$JOY_NAME"
    return
  fi
  echo "0"
}
