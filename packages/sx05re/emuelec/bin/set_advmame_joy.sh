#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

# Configure ADVMAME players based on ES settings
CONFIG_DIR="/storage/.advance"
CONFIG="${CONFIG_DIR}/advmame.rc"
ES_FEATURES="/storage/.config/emulationstation/es_features.cfg"

#source /storage/joy_common.sh "advmame"
source /usr/bin/joy_common.sh "advmame"

ROMNAME=$1


BTN_CFG="0 1 2 3 4 5 6 7"

BTN_H0=$(get_ee_setting advmame_btn_h0)
[[ -z "$BTN_H0" ]] && BTN_H0=4


declare -A ADVMAME_VALUES=(
  ["b0"]="button1"
  ["b1"]="button2"
  ["b2"]="button3"
  ["b3"]="button4"
  ["b4"]="button5"
  ["b5"]="button6"
  ["b6"]="button7"
  ["b7"]="button8"
  ["b8"]="button9"
  ["b9"]="button10"
  ["b10"]="button11"
  ["b11"]="button12"
  ["b12"]="button13"
  ["b13"]="button14"
  ["b14"]="button15"
  ["b15"]="button16"
  ["b16"]="button17"
  ["h0.1"]="stick${BTN_H0},y,up"
  ["h0.4"]="stick${BTN_H0},y,down"
  ["h0.8"]="stick${BTN_H0},x,left"
  ["h0.2"]="stick${BTN_H0},x,right"
  ["a0,1"]="stick,y,up"
  ["a0,2"]="stick,y,down"
  ["a1,1"]="stick,x,left"
  ["a1,2"]="stick,x,right"
)

declare GC_ORDER=(
  "a"
  "b"
  "x"
  "y"
  "rightshoulder"
  "leftshoulder"
  "righttrigger"
  "lefttrigger"
)

declare -A GC_NAMES=()

get_button_cfg() {
	local BTN_INDEX=$(get_ee_setting "joy_btn_cfg" "mame" "${ROMNAME}")
  [[ -z $BTN_INDEX ]] && BTN_INDEX=$(get_ee_setting "mame.joy_btn_cfg")

  if [[ ! -z $BTN_INDEX ]] && [[ $BTN_INDEX -gt 0 ]]; then
		local BTN_SETTING="AdvanceMame.joy_btn_order$BTN_INDEX"
    local BTN_CFG_TMP="$(get_ee_setting $BTN_SETTING)"
		[[ ! -z $BTN_CFG_TMP ]] && BTN_CFG="${BTN_CFG_TMP}"
	fi
	echo "$BTN_CFG"
}

# Cleans all the inputs for the gamepad with name $GAMEPAD and player $1 
clean_pad() {
#echo "Cleaning pad $1 $2" #debug
	sed -i "/device_joystick.*/d" ${CONFIG}
	sed -i "/input_map\[p${1}_.*/d" ${CONFIG}
	sed -i "/input_map\[coin${1}.*/d" ${CONFIG}
	sed -i "/input_map\[start${1}.*/d" ${CONFIG}

  if [[ "${1}" == "1" ]]; then
    sed -i '/input_map\[ui_[[:alpha:]]*\].*/d' ${CONFIG}
  fi
	echo "device_joystick sdl" >> ${CONFIG}
}

# Sets pad depending on parameters $GAMEPAD = name $1 = player
set_pad(){
<<<<<<< HEAD
#echo "Setting pad $1 from ${GPFILE}" #debug
	COIN=$(cat "${GPFILE}" | grep -E 'input_select_btn' | cut -d '"' -f2) 
	COIN=$((COIN+1))
	echo "input_map[coin${1}] joystick_button[${GAMEPAD},button${COIN}]" >> ${CONFIG}
	START=$(cat "${GPFILE}" | grep -E 'input_start_btn' | cut -d '"' -f2)
	START=$((START+1))
	echo "input_map[start${1}] joystick_button[${GAMEPAD},button${START}]" >> ${CONFIG}


button=""
i=1
for bi in ${BTN_CFG}; do
	button="${BTN_ORDER[$bi]}"
	KEY=$(cat "${GPFILE}" | grep -E "${button}" | cut -d '"' -f2)
if [ ! -z "$KEY" ]; then 
	KEY=$((KEY+1))
case "${button}" in 
	"input_up_btn")
	echo "input_map[p${1}_up] joystick_button[${GAMEPAD},button${KEY}] or joystick_digital[${GAMEPAD},stick,y,up]" >> ${CONFIG}
	[[ "${1}" == "1" ]] && echo "input_map[ui_up] joystick_button[${GAMEPAD},button${KEY}] or joystick_digital[${GAMEPAD},stick,y,up]" >> ${CONFIG}
		;;
	"input_down_btn")
	echo "input_map[p${1}_down] joystick_button[${GAMEPAD},button${KEY}] or joystick_digital[${GAMEPAD},stick,y,down]" >> ${CONFIG}
	[[ "${1}" == "1" ]] && echo "input_map[ui_down] joystick_button[${GAMEPAD},button${KEY}] or joystick_digital[${GAMEPAD},stick,y,down]" >> ${CONFIG}
		;;
	"input_left_btn")
	echo "input_map[p${1}_left] joystick_button[${GAMEPAD},button${KEY}] or joystick_digital[${GAMEPAD},stick,x,left]" >> ${CONFIG}
	[[ "${1}" == "1" ]] && echo "input_map[ui_left] joystick_button[${GAMEPAD},button${KEY}] or joystick_digital[${GAMEPAD},stick,x,left]" >> ${CONFIG}
		;;
	"input_right_btn")
	echo "input_map[p${1}_right] joystick_button[${GAMEPAD},button${KEY}] or joystick_digital[${GAMEPAD},stick,x,right]" >> ${CONFIG}
	[[ "${1}" == "1" ]] && echo "input_map[ui_right] joystick_button[${GAMEPAD},button${KEY}] or joystick_digital[${GAMEPAD},stick,x,right]" >> ${CONFIG}
		;;
	*)
	echo "input_map[p${1}_button${i}] joystick_button[${GAMEPAD},button${KEY}]" >> ${CONFIG}
	i=$((i+1))
	;;
esac
if [[ -f "$CONFIG_DIR/ADD_DPAD" ]]; then
	STICK_VALUE=`cat $CONFIG_DIR/ADD_DPAD`
	if [[ $STICK_VALUE =~ ^[0-9]{1}$ ]]; then
		for direction in up down left right; do
			AXIS="y"
			[[ "$direction" =~ ^(left|right)$ ]] && AXIS="x"
			if [[ "${button}" == "input_${direction}_btn" ]]; then
				INPUT_MAP_VAL=`cat ${CONFIG} | grep "input_map\[p${1}_${direction}\]"`
				sed -i "/input_map\[p${1}_${direction}\].*/d" ${CONFIG}
				echo "${INPUT_MAP_VAL} or joystick_digital[${GAMEPAD},stick${STICK_VALUE},${AXIS},${direction}]" >> ${CONFIG}
				if [[ "${1}" == "1" ]]; then
					INPUT_MAP_VAL=`cat ${CONFIG} | grep "input_map\[ui_${direction}\]"`
					sed -i "/input_map\[ui_${direction}\].*/d" ${CONFIG}
					echo "${INPUT_MAP_VAL} or joystick_digital[${GAMEPAD},stick${STICK_VALUE},${AXIS},${direction}]" >> ${CONFIG}
				fi
			fi
		done  
	fi
fi
fi
done

# Menu should only be set to player 1
if [[ "${1}" == "1" ]]; then	
#echo "Setting menu buttons for player 1" #debug

	BSELECT=$(cat "${GPFILE}" | grep -E 'input_a_btn' | cut -d '"' -f2)
if [ ! -z "$BSELECT" ]; then 
	BSELECT=$((BSELECT+1))
	echo "input_map[ui_select] keyboard[0,enter] or keyboard[1,enter] or joystick_button[${GAMEPAD},button${BSELECT}]" >> ${CONFIG}
fi
	MENU=$(cat "${GPFILE}" | grep -E 'input_r3_btn' | cut -d '"' -f2)
if [ ! -z "$MENU" ]; then 
	MENU=$((MENU+1))
	echo "input_map[ui_cancel] keyboard[0,backspace] or keyboard[1,backspace] or joystick_button[${GAMEPAD},button${MENU}]" >> ${CONFIG}
fi
	CONFIGURE=$(cat "${GPFILE}" | grep -E 'input_l3_btn' | cut -d '"' -f2)
if [ ! -z "$CONFIGURE" ]; then 
	CONFIGURE=$((CONFIGURE+1))
	echo "input_map[ui_configure] keyboard[1,tab] or keyboard[0,tab] or joystick_button[${GAMEPAD},button${CONFIGURE}]" >> ${CONFIG}	
fi
fi
	}

# Search for connected gamepads based on parameters $1 = player number $2 = device number e.g js0 and extract the name to $GAMEPAD
find_gamepad() {
for file in /tmp/joypads/*.cfg; do
	EE_GAMEPAD=$(cat "$file" | grep input_device|  cut -d'"' -f 2)
	ES_EE_GAMEPAD=$(printf %q "$EE_GAMEPAD")
if cat /proc/bus/input/devices | grep -Ew -A 4 -B 1 "Name=\"${ES_EE_GAMEPAD}" | grep ${2} > /dev/null; then
	FOUND=1
	GPFILE="$file"
	GAMEPAD=$(echo "$EE_GAMEPAD" | sed "s|,||g" | sed "s|_||g" | cut -d'"' -f 2 | sed "s|(||" | sed "s|)||" | sed -e 's/[^A-Za-z0-9._-]/ /g' | sed 's/[[:blank:]]*$//' | sed 's/-//' | sed -e 's/[^A-Za-z0-9._-]/_/g' |tr '[:upper:]' '[:lower:]' | tr -d '.')

# check to see if the gamepad is exactly the same, if it is set a number after the gamepad, unfortunately this will be set according to the jsX as I do not know how to diferentiate from them	
	if [[ "$GAMEPAD" == "$FIRST_GAMEPAD" ]]; then
		GAMEPAD="$GAMEPAD"_${1}
	fi
[[ -z "${FIRST_GAMEPAD}" ]] && FIRST_GAMEPAD="$GAMEPAD"
	break
else
	FOUND=0
fi
done

if [ ${FOUND} = 1 ]; then
#echo "setting gamepad $GAMEPAD as player $1 on $2" #debug
	clean_pad "${1}" "$GAMEPAD"
	set_pad "${1}"
else
	clean_pad "${1}"
fi
}

# This will extract the GUID from es_settings.cfg depending on how many players have been set on ES and determine if they are currently connected to the device.
get_players() {
# You can set up to 8 player on ES
for ((y = 1; y <= 8; y++)); do
#echo "Player $y" #debug
#echo "Getting GUID for INPUT P${y}GUID" #debug

DEVICE_GUID=$(get_es_setting string "INPUT P${y}GUID")
	if [[ ! -z "${DEVICE_GUID}" ]]; then
		v=${DEVICE_GUID:0:8}
		part1=$(echo ${v:6:2}${v:4:2}${v:2:2}${v:0:2}) # Bus, generally not needed
		v=${DEVICE_GUID:8:8}
		part2=$(echo ${v:6:2}${v:4:2}${v:2:2}${v:0:2}) # Vendor
		v=${DEVICE_GUID:16:8}
		part3=$(echo ${v:6:2}${v:4:2}${v:2:2}${v:0:2}) # Product
		v=${DEVICE_GUID:24:8}
		part4=$(echo ${v:6:2}${v:4:2}${v:2:2}${v:0:2}) # Version

		input_vendor=$(echo ${part2:4})
		input_product=$(echo ${part3:4})
		input_version=$(echo ${part4:4})

		EE_DEV=$(cat /proc/bus/input/devices | grep -Ew -A 6 "Vendor=${input_vendor}" | grep -Ew -A 6 "Product=${input_product}" | grep -Ew -A 6 "Version=${input_version}" | grep -Ew "H: Handlers=.*js.*")
		if [[ ! -z "${EE_DEV}" ]]; then
		JOYSTICK="${EE_DEV##*js}"  # read from -P onwards
		JOYSTICK="${JOYSTICK%% *}"  # until a space is found
		
#echo "${y}" "js${JOYSTICK##*js}" #debug
			PAD_FOUND=1
			find_gamepad "${y}" "js${JOYSTICK##*js}"
		else
			EE_DEV=""
		fi
	fi
done
=======
>>>>>>> 2b8f4ed12f6637b727226dafb75d1d966cab667a

  local DEVICE_GUID=$3
  local JOY_NAME="$4"



  local GC_CONFIG=$(cat "$GCDB" | grep "$DEVICE_GUID" | grep "platform:Linux" | head -1)
  echo "GC_CONFIG=$GC_CONFIG"
  [[ -z $GC_CONFIG ]] && return

  [[ -z "$JOY_NAME" ]] && JOY_NAME=$(echo $GC_CONFIG | cut -d',' -f2)
  [[ -z "$JOY_NAME" ]] && return

  local GAMEPAD=$(echo "$JOY_NAME" | sed "s|(||" | sed "s|)||" \
    | sed -e 's/[^A-Za-z0-9._-]/ /g' | sed 's/[[:blank:]]*$//' | sed 's/-//' \
    | sed -e 's/[^A-Za-z0-9._-]/_/g' | tr '[:upper:]' '[:lower:]' | tr -d '.')

  local NAME_NUM="${GC_NAMES[$JOY_NAME]}"
  if [[ -z "NAME_NUM" ]]; then
    GC_NAMES[$JOY_NAME]=1
  else
    GC_NAMES[$JOY_NAME]=$(( NAME_NUM+1 ))
  fi
	[[ "$1" != "1" && "$NAME_NUM" -gt "1" ]] && GAMEPAD="${GAMEPAD}_${NAME_NUM}"

  local GC_MAP=$(echo $GC_CONFIG | cut -d',' -f3-)

  declare DIRS=()
  declare -A DIR_INDEX=(
    [dpup]="0"
    [dpdown]="1"
    [dpleft]="2"
    [dpright]="3"
    [leftx]="0,1"
    [lefty]="2,3"
  )

  local i=1
  set -f
  local GC_ARRAY=(${GC_MAP//,/ })
  declare -A GC_ASSOC=()
  for index in "${!GC_ARRAY[@]}"
  do
      local REC=${GC_ARRAY[$index]}
      local GC_INDEX=$(echo $REC | cut -d ":" -f 1)
      [[ $GC_INDEX == "" || $GC_INDEX == "platform" ]] && continue

      local TVAL=$(echo $REC | cut -d ":" -f 2)
      GC_ASSOC["$GC_INDEX"]=$TVAL

      [[ " ${GC_ORDER[*]} " == *" ${GC_INDEX} "* ]] && continue
      local BUTTON_VAL=${TVAL:1}
      local BTN_TYPE=${TVAL:0:1}
      local VAL="${ADVMAME_VALUES[${TVAL}]}"
      local I="${DIR_INDEX[$GC_INDEX]}"
      local DIR="${DIRS[$I]}"

      # Create Axis Maps
      case $GC_INDEX in
        dpup|dpdown|dpleft|dpright)
          [[ ! -z "$DIR" ]] && DIR+= " or "
          DIR+="joystick_button[${GAMEPAD},${VAL}]"
          DIRS["$I"]=$DIR
          ;;
        leftx|lefty)
          for i in {1..2}; do
            I=$(echo "${DIR_INDEX[$GC_INDEX]}" | cut -d, -f "$i")
            DIR="${DIRS[$I]}"
            [[ ! -z "$DIR" ]] && DIR+=" or "
            VAL="${ADVMAME_VALUES[${TVAL},${i}]}"
            DIR+="joystick_digital[${GAMEPAD},${VAL}]"
            DIRS["$I"]=$DIR
          done
          ;;
        start)
          echo "input_map[start${1}] joystick_button[${GAMEPAD},${VAL}]" >> ${CONFIG}
          ;;
        back)
          echo "input_map[coin${1}] joystick_button[${GAMEPAD},${VAL}]" >> ${CONFIG}
          ;;
      esac
  done

  declare -i i=1
  for bi in ${BTN_CFG}; do
    local button="${GC_ORDER[$bi]}"
    [[ -z "$button" ]] && continue
    button="${GC_ASSOC[$button]}"
    local VAL="${ADVMAME_VALUES[$button]}"
    echo "input_map[p${1}_button${i}] joystick_button[${GAMEPAD},${VAL}]" >> ${CONFIG}
    (( i++ ))
  done

  echo "input_map[p${1}_up] ${DIRS[0]}" >> ${CONFIG}
  echo "input_map[p${1}_down] ${DIRS[1]}" >> ${CONFIG}
  echo "input_map[p${1}_left] ${DIRS[2]}" >> ${CONFIG}
  echo "input_map[p${1}_right] ${DIRS[3]}" >> ${CONFIG}

  # Menu should only be set to player 1
  if [[ "${1}" == "1" ]]; then
  #echo "Setting menu buttons for player 1" #debug
    echo "input_map[ui_up] ${DIRS[0]}" >> ${CONFIG}
    echo "input_map[ui_down] ${DIRS[1]}" >> ${CONFIG}
    echo "input_map[ui_left] ${DIRS[2]}" >> ${CONFIG}
    echo "input_map[ui_right] ${DIRS[3]}" >> ${CONFIG}

    local button="${GC_ASSOC[a]}"
    local VAL="${ADVMAME_VALUES[$button]}"
    if [ ! -z "$VAL" ]; then
      echo "input_map[ui_select] keyboard[0,enter] or keyboard[1,enter] or joystick_button[${GAMEPAD},${VAL}]" >> ${CONFIG}
    fi
    button="${GC_ASSOC[leftstick]}"
    VAL="${ADVMAME_VALUES[$button]}"
    if [ ! -z "$VAL" ]; then
      echo "input_map[ui_cancel] keyboard[0,backspace] or keyboard[1,backspace] or joystick_button[${GAMEPAD},${VAL}]" >> ${CONFIG}
    fi
    button="${GC_ASSOC[rightstick]}"
    VAL="${ADVMAME_VALUES[$button]}"
    if [ ! -z "$VAL" ]; then
      echo "input_map[ui_configure] keyboard[1,tab] or keyboard[0,tab] or joystick_button[${GAMEPAD},${VAL}]" >> ${CONFIG}
    fi
  fi
}

ADVMAME_REGEX="<emulator.*name\=\"AdvanceMame\".*features\=\".*[ ,]{1}joybtnremap[, \"]{1}.*\".*/>$"
ADVMAME_REMAP=$(cat "${ES_FEATURES}" | grep -E "$ADVMAME_REGEX")
[[ ! -z "$ADVMAME_REMAP" ]] && BTN_CFG=$(get_button_cfg)
echo "BTN_CFG=$BTN_CFG"

jc_get_players
