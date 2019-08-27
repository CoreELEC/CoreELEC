#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# WARNING !!!! This file has been auto generated, please do not modify!

check_reboot() { 
if [ "$1" == "reboot" ]; then
	sync
	systemctl reboot
fi 
}

EE_VERSION1=$(cat /storage/.config/EE_VERSION)
EE_VERSION2=$(cat /usr/config/EE_VERSION)

 if [ "$EE_VERSION1" != "$EE_VERSION2" ]; then

# We make sure emulationstation is not running
	if  pgrep emulationstation >/dev/null ; then
		sleep 1
	fi
	
 cp -rf /usr/config/emulationstation/es_systems.cfg /storage/.emulationstation/es_systems.cfg
 rm /emuelec/scripts/modules/*
