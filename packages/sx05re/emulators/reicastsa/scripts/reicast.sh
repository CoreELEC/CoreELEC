#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

REICASTBIN="/usr/bin/reicast"

/emuelec/scripts/setres.sh 16

#set reicast BIOS dir to point to /storage/roms/bios/dc
if [ ! -L /storage/.local/share/reicast/data ]; then
	mkdir -p /storage/.local/share/reicast 
	rm -rf /storage/.local/share/reicast/data
	ln -s /storage/roms/bios/dc /storage/.local/share/reicast/data
fi

if [ ! -L /storage/.local/share/reicast/mappings ]; then
mkdir -p /storage/.local/share/reicast/
ln -sf /storage/.config/reicast/mappings /storage/.local/share/reicast/mappings
fi


# try to automatically set the gamepad in emu.cfg
y=1


for D in `find /dev/input/by-id/ | grep -e event-joystick -e amepad`; do
  str=$(ls -la $D)
  i=$((${#str}-1))
  DEVICE=$(echo "${str:$i:1}")
  CFG="/storage/.config/reicast/emu.cfg"
   sed -i -e "s/^evdev_device_id_$y =.*$/evdev_device_id_$y = $DEVICE/g" $CFG
 # echo "Device: $y input $DEVICE"
  y=$((y+1))
 if [$y -lt 4]; then
  break
 fi 
done

set_audio alsa

[[ -f "/ee_s905" ]] && mv /storage/.config/asound.conf /storage/.config/asound.confs
${REICASTBIN} "$1" &>/dev/null
[[ -f "/ee_s905" ]] && mv /storage/.config/asound.confs /storage/.config/asound.conf

/emuelec/scripts/setres.sh
 
