#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

JSLISTENCONF="/emuelec/configs/jslisten.cfg"
sed -i '/program=.*/d' ${JSLISTENCONF}
echo "program=\"/usr/bin/killall VVVVVV\"" >> ${JSLISTENCONF}

# JSLISTEN setup so that we can kill VVVVVV using hotkey+start
/storage/.emulationstation/scripts/configscripts/z_getkillkeys.sh
/emuelec/bin/jslisten &

set_audio alsa
# VVVVVV will complain about a missing gamecontrollerdb.txt unless we switch to this folder first
cd /storage/.config/SDL-GameControllerDB/
VVVVVV
set_audio pulseaudio

killall jslisten
