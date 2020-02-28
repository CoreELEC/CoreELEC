#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

JSLISTENCONF="/emuelec/configs/jslisten.cfg"
sed -i "2s|program=.*|program=\"/usr/bin/killall devilutionx\"|" ${JSLISTENCONF}

# If jslisten is running we kill it first so that it can reload the config file. 
[ pgrep -f "/emuelec/bin/jslisten" >/dev/null 2>&1 ] &&  killall jslisten

# JSLISTEN setup so that we can kill CGeniusExe using hotkey+start
/storage/.emulationstation/scripts/configscripts/z_getkillkeys.sh
/emuelec/bin/jslisten --mode hold &

if [ ! -L /storage/.local/share/diasurgical/devilution/diabdat.mpq ]; then
mkdir -p /storage/.local/share/diasurgical/devilution/
ln -sf /storage/roms/ports/diablo/diabdat.mpq /storage/.local/share/diasurgical/devilution/diabdat.mpq
fi

cd /emuelec/bin/
./devilutionx

# Kill jslisten, we don't need to but just to make sure, dot not kill if using OdroidGoAdvance
[[ "$EE_DEVICE" != "OdroidGoAdvance" ]] && killall jslisten
