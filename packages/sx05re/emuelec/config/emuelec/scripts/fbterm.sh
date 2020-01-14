#!/usr/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

source /emuelec/scriptmodules/helpers.sh

if [ -e /proc/device-tree/t82x@d00c0000/compatible ]; then
	/emuelec/scripts/setres.sh 16
fi


if [[ "$1" == *"13 - Launch Terminal (kb).sh"* ]]; then
fbterm -s 32 --verbose < /dev/tty1
else
fbterm "$1" -s 24 < /dev/tty1
fi 

joy2keyStop
