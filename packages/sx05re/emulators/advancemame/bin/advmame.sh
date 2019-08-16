#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

CONFIG_DIR="/storage/.advance"

if [ ! -d "$CONFIG_DIR" ]; then
 mkdir -p $CONFIG_DIR
 cp -rf /usr/share/advance/* $CONFIG_DIR/
fi

if [[ "$1" = *"roms/arcade"* ]]; then 
sed -i "s|/roms/mame|/roms/arcade|g" $CONFIG_DIR/advmame.rc
 else
sed -i "s|/roms/arcade|/roms/mame|g" $CONFIG_DIR/advmame.rc
fi 

# Configure P1 gamepad based on js0
/emuelec/scripts/set_advmame_joy.sh

ARG=$(echo basename $1 | sed 's/\.[^.]*$//')
ARG="$(echo $1 | sed 's=.*/==;s/\.[^.]*$//')"         

SDL_AUDIODRIVER=alsa /usr/bin/advmame $ARG -quiet


