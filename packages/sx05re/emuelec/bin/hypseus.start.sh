#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

dir="${1}"
name=${dir##*/}
name=${name%.*}
config="/storage/.config/emuelec/configs/hypseus"
configfile="${config}/hypinput.ini"

# Attempt to auto configure gamepad
#
GAMEPADINFO="$(sdljoytest -skip_loop)"
#JOYGUID=$(echo "${GAMEPADINFO}" | grep "Joystick 0 Guid " | sed "s|Joystick 0 Guid ||")
JOYNAME=$(echo "${GAMEPADINFO}" | grep "Joystick 0 name " | sed "s|Joystick 0 name ||" | sed "s|'||g")
#

for file in /tmp/joypads/*.cfg; do
	GAMEPAD=$(cat "$file" | grep input_device|  cut -d'"' -f 2)
if [ "${JOYNAME}" == "${GAMEPAD}" ]; then
	GPFILE="${file}"

# Other keys to consider KEY_SCREENSHOT KEY_QUIT KEY_PAUSE
for key in KEY_UP KEY_DOWN KEY_LEFT KEY_RIGHT KEY_BUTTON1 KEY_BUTTON2 KEY_BUTTON3 KEY_START1 KEY_COIN1; do 

    case ${key} in
        "KEY_UP")
           button=$(cat "${GPFILE}" | grep -E 'input_up_btn' | cut -d '"' -f2)
           keyboard="1073741906 0"
        ;;
        "KEY_DOWN")
           button=$(cat "${GPFILE}" | grep -E 'input_down_btn' | cut -d '"' -f2)
           keyboard="1073741905 0"
        ;;
        "KEY_LEFT")
           button=$(cat "${GPFILE}" | grep -E 'input_left_btn' | cut -d '"' -f2)
           keyboard="1073741904 0"
        ;;
        "KEY_RIGHT")
           button=$(cat "${GPFILE}" | grep -E 'input_right_btn' | cut -d '"' -f2)
           keyboard="1073741903 0"
        ;;
        "KEY_BUTTON1")
           button=$(cat "${GPFILE}" | grep -E 'input_a_btn' | cut -d '"' -f2)
           keyboard="1073742048 0"
        ;;
        "KEY_BUTTON2")
           button=$(cat "${GPFILE}" | grep -E 'input_b_btn' | cut -d '"' -f2) 
           keyboard="1073742050 0"
        ;;
        "KEY_BUTTON3")
           button=$(cat "${GPFILE}" | grep -E 'input_x_btn' | cut -d '"' -f2) 
           keyboard="32 0"
        ;;
        "KEY_START1")
           button=$(cat "${GPFILE}" | grep -E 'input_start_btn' | cut -d '"' -f2) 
           keyboard="49 0"
        ;;
        "KEY_COIN1")
           button=$(cat "${GPFILE}" | grep -E 'input_select_btn' | cut -d '"' -f2) 
           keyboard="53 54"
        ;;
    esac

# if the button is in fact a hat extract the number, else use the button number+1
if [[ "${button}" == "h"* ]]; then
    button="0 ${button:1:1}"
else
    button="$((${button} + 1))"
fi

sed -i "s|${key}.*|${key} = ${keyboard} ${button} |" ${configfile}

done 
fi 
done # finish auto gamepad


if [[ -f "${dir}/${name}.commands" ]]; then
    params=$(<"${dir}/${name}.commands")
fi

cd "${config}"

if [[ -f "${dir}/${name}.singe" ]]; then
    hypseus singe vldp -framefile "${dir}/${name}.txt" -script "${dir}/${name}.singe" -fullscreen -useoverlaysb 2 $params
else
    hypseus "${name}" vldp -framefile "${dir}/${name}.txt" -fullscreen -useoverlaysb 2 $params
fi


