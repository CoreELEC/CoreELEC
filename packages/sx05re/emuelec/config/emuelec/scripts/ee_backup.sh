#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Usage:
# ee_backup.sh b -- backup configurations 
# ee_backup.sh r -- restore configurations 

BACKUPFILE="/storage/downloads/ee_backup_config.zip"
mkdir -p "/storage/downloads/"

case "$1" in
"b")
systemctl stop emustation
zip ${BACKUPFILE} /storage/.cache/bluetooth/* /emuelec/configs/emuoptions.conf /emuelec/configs/emuelec.conf /storage/.emulationstation/es_*.cfg /tmp/joypads/* /storage/.config/retroarch/*.cfg
sleep 3
systemctl start emustation
;;
"r")
systemctl stop emustation
unzip -o ${BACKUPFILE} -d /
sleep 3
systemctl start emustation
;;
esac





