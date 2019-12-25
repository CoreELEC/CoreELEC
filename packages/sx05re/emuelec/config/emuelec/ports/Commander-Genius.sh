#!/bin/bash

# Source predefined functions and variables
. /etc/profile

JSLISTENCONF="/emuelec/configs/jslisten.cfg"
sed -i '/program=.*/d' ${JSLISTENCONF}
echo "program=\"/usr/bin/killall CGeniusExe\"" >> ${JSLISTENCONF}

# JSLISTEN setup so that we can kill CGeniusExe using hotkey+start
/storage/.emulationstation/scripts/configscripts/z_getkillkeys.sh

[[ ! -L "/storage/.CommanderGenius" ]] && ln -sf /emuelec/configs/CommanderGenius /storage/.CommanderGenius

set_audio alsa

/emuelec/bin/CGeniusExe

set_audio pulseaudio
