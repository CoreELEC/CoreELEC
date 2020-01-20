#!/bin/bash

# Source predefined functions and variables
. /etc/profile

JSLISTENCONF="/emuelec/configs/jslisten.cfg"
sed -i '/program=.*/d' ${JSLISTENCONF}
echo "program=\"/usr/bin/killall VVVVVV\"" >> ${JSLISTENCONF}

# JSLISTEN setup so that we can kill VVVVVV using hotkey+start
/storage/.emulationstation/scripts/configscripts/z_getkillkeys.sh

set_audio alsa
# VVVVVV will complain about a missing gamecontrollerdb.txt unless we switch to this folder first
cd /storage/.config/SDL-GameControllerDB/
VVVVVV
set_audio pulseaudio
