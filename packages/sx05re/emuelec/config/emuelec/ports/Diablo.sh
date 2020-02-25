#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

JSLISTENCONF="/emuelec/configs/jslisten.cfg"
sed -i '/program=.*/d' ${JSLISTENCONF}
echo "program=\"/usr/bin/killall devilutionx\"" >> ${JSLISTENCONF}

# JSLISTEN setup so that we can kill devilutionx using hotkey+start
/storage/.emulationstation/scripts/configscripts/z_getkillkeys.sh
/emuelec/bin/jslisten &

if [ ! -L /storage/.local/share/diasurgical/devilution/diabdat.mpq ]; then
mkdir -p /storage/.local/share/diasurgical/devilution/
ln -sf /storage/roms/ports/diablo/diabdat.mpq /storage/.local/share/diasurgical/devilution/diabdat.mpq
fi

cd /emuelec/bin/
./devilutionx

killall jslisten
