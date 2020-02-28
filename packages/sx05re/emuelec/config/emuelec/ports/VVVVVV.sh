#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

JSLISTENCONF="/emuelec/configs/jslisten.cfg"
sed -i "2s|program=.*|program=\"/usr/bin/killall VVVVVV\"|" ${JSLISTENCONF}

# If jslisten is running we kill it first so that it can reload the config file. 
[ pgrep -f "/emuelec/bin/jslisten" >/dev/null 2>&1 ] &&  killall jslisten

# JSLISTEN setup so that we can kill CGeniusExe using hotkey+start
/storage/.emulationstation/scripts/configscripts/z_getkillkeys.sh
/emuelec/bin/jslisten --mode hold &

set_audio alsa
# VVVVVV will complain about a missing gamecontrollerdb.txt unless we switch to this folder first
cd /storage/.config/SDL-GameControllerDB/
VVVVVV
set_audio default

# Kill jslisten, we don't need to but just to make sure, dot not kill if using OdroidGoAdvance
[[ "$EE_DEVICE" != "OdroidGoAdvance" ]] && killall jslisten
