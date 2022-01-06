#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

mkdir -p "/emuelec/configs/yabasanshiro"

if [ ! -e "/emuelec/configs/yabasanshiro/input.cfg" ]; then
    cp -rf "/usr/config/emuelec/configs/yabasanshiro/input.cfg" "/emuelec/configs/yabasanshiro/input.cfg"
fi

yabasanshiro -i "${1}" -b "/storage/roms/bios/saturn_bios.bin" -r 2 -a -nf > /emuelec/logs/emuelec.log 2>&1
