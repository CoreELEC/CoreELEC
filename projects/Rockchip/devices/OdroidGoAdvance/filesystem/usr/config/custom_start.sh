#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

# Place any scripts you need to run at boot on this file

case "${1}" in
"before")
# Any commands that you want to run before the frontend begins should go here

OGA=$(cat /proc/device-tree/compatible)
[[ -f "/storage/joypad_done" ]] && DONE=$(cat /storage/joypad_done) || DONE="no"

if [[ "${OGA}" == *"v11"* ]]; then
	# "V1.1"
if [[ "${DONE}" != "v11" ]]; then
	echo "v11" > /storage/joypad_done
	cp -rf /emuelec/configs/es_oga_v11.cfg /storage/.emulationstation/es_input.cfg
	cp -rf /emuelec/configs/ra_oga_v11.cfg /tmp/joypads/odroidgo2_joypad.cfg
fi
else
	# "V1.0"
if [[ "${DONE}" != "v10" ]]; then
	echo "v10" > /storage/joypad_done
	cp -rf /emuelec/configs/es_oga.cfg /storage/.emulationstation/es_input.cfg
	cp -rf /emuelec/configs/ra_oga.cfg /tmp/joypads/odroidgo2_joypad.cfg
fi
fi
	exit 0
	;;
"after")
# Any commands that you want to run after the frontend has started goes here

    exit 0
	;;
esac
## nothing was called so exit
exit 0
