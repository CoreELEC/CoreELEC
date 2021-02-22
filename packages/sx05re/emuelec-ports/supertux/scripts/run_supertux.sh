#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

DATA="https://github.com/EmuELEC/supertux/archive/data_only.zip"
DATAFOLDER="/storage/roms/ports/supertux"

mkdir -p "${DATAFOLDER}"
cd "${DATAFOLDER}"

if [ "$EE_DEVICE" == "Amlogic-ng" ]; then 
fbfix
fi

if [ ! -e "${DATAFOLDER}/credits.stxt" ]; then
    text_viewer -y -f 24 -t "Data does not exists!" -m "It seems this is the first time you are launching Super Tux or the data folder does not exists\n\nData is about 200 MB total, and you need to be connected to the internet\n\nDownload and continue?"
        if [[ $? == 21 ]]; then
            ee_console enable
            wget "${DATA}" -q --show-progress > /dev/tty0 2>&1
            unzip "data_only.zip" > /dev/tty0
            mv supertux-data_only/data/* "${DATAFOLDER}" > /dev/tty0
            rm -rf "supertux-data_only"
            rm "data_only.zip"
            rm "imgui.ini"
            ee_console disable
           SUPERTUX2_DATA_DIR="${DATAFOLDER}" supertux2
        else
            exit 0
        fi
else
    SUPERTUX2_DATA_DIR="${DATAFOLDER}" supertux2
fi

