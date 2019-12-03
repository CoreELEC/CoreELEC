#!/bin/bash

# Source predefined functions and variables
. /etc/profile

[[ ! -L "/storage/.CommanderGenius" ]] && ln -sf /emuelec/configs/CommanderGenius /storage/.CommanderGenius

set_audio alsa

/emuelec/bin/CGeniusExe

set_audio pulseaudio
