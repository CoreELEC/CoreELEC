#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

CONFIG_DIR="/storage/.config/residualvm"
BINDIR="/usr/bin"
GAME="$(echo $2 | sed 's=.*/==;s/\.[^.]*$//')" 

create_rvm(){
	$BINDIR/residualvm --list-targets | tail -n +3 | cut -d " " -f 1 | while read line; do
    id=($line);
    touch "${CONFIG_DIR}/games/$id.rvm"
	done 
	}

if [ ! -d "$CONFIG_DIR" ]; then
 mkdir -p $CONFIG_DIR
 cp -rf /usr/config/residualvm/* $CONFIG_DIR/
fi

if [ -d "/storage/.config/residualvm/extra" ]; then 
EXTRA="--extrapath=/storage/.config/residualvm/extra"
fi 

case $1 in
"add") 
$BINDIR/residualvm --add --path="/storage/roms/residualvm" --recursive
;;
"create") 
create_rvm
;;
*) 
set_audio "fluidsynth"
[[ ! -f "/ee_s905" ]] && /storage/.config/emuelec/bin/fbfix
$BINDIR/residualvm --fullscreen --joystick=0 $EXTRA "$GAME"
set_audio "pulseaudio"
;;
esac 
