#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# OpenBOR only works with Pak files, if you have an extracted game you will need to create a pak first.
# If master.cfg does not work, sound is weird or controller not working, you will need to use a keyboard to set your gamepad
# after you set up your gamne, copy the /storage/.config/openbor/Saves/{gamename}.cfg file to /storage/.config/openbor/master.cfg
# master.cfg will only be copied the first time you run that particular game.

/emuelec/scripts/setres.sh 16

pakname=$(basename "$1")
pakname="${pakname%.*}"

echo $pakname
# Make sure the folders exists
	mkdir -p /storage/.config/openbor/Paks
	mkdir -p /storage/.config/openbor/Saves

# copy pak to Paks folder
	cp "$1" /storage/.config/openbor/Paks

# only copy master.cfg if its the first time running the pak
	if [ ! -f "/storage/.config/openbor/Saves/${pakname}.cfg" ]; then
		cp "/storage/.config/openbor/master.cfg" "/storage/.config/openbor/Saves/${pakname}.cfg"
	fi

# Run OpenBOR in the config folder
    cd /storage/.config/openbor/
	SDL_AUDIODRIVER=alsa OpenBOR

# Delete Pak from temp folder
	rm -rf /storage/.config/openbor/Paks/*

/emuelec/scripts/setres.sh
