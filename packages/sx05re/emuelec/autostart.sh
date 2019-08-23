#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# DO NOT modify this file, if you need to use autostart please use /storage/.config/custom_start.sh 

# Check if we have unsynched update files
/usr/config/emuelec/scripts/force_update.sh

/emuelec/scripts/setres.sh

# It seems some slow SDcards have a problem creating the symlink on time :/
CONFIG_DIR="/storage/.emulationstation"
CONFIG_DIR2="/storage/.config/emulationstation"

if [ ! -L "$CONFIG_DIR" ]; then
ln -sf $CONFIG_DIR2 $CONFIG_DIR
fi

# Show splash creen 
/emuelec/scripts/show_splash.sh intro

# Clean cache garbage when boot up.
rm -rf /storage/.cache/cores/*

# Is BMG enabled? then start the music
if [ -f /storage/.emulationstation/es_settings.cfg ]; then

DEFE=$(sed -n 's|\s*<string name="EmuELEC_BGM_BOOT" value="\(.*\)" />|\1|p' $CONFIG_DIR/es_settings.cfg)

case "$DEFE" in
"Yes")
	killall mpg123
	/storage/.emulationstation/scripts/bgm.sh start
	sed -i -e "s/name=\"BGM\" value=\"false\"/name=\"BGM\" value=\"true\"/" $CONFIG_DIR/es_settings.cfg
	;;
"No")
	killall mpg123
	sed -i -e "s/name=\"BGM\" value=\"true\"/name=\"BGM\" value=\"false\"/" $CONFIG_DIR/es_settings.cfg
	;;
esac

fi

# handle SSH
DEFE=$(sed -n 's|\s*<bool name="SSH" value="\(.*\)" />|\1|p' $CONFIG_DIR/es_settings.cfg)

case "$DEFE" in
"true")
	mkdir -p /storage/.cache/services/
	touch /storage/.cache/services/sshd.conf
	systemctl start sshd
	;;
"false")
	systemctl stop sshd
	rm /storage/.cache/services/sshd.conf
	;;
esac

# What to start at boot?
DEFE=$(sed -n 's|\s*<string name="EmuELEC_BOOT" value="\(.*\)" />|\1|p' $CONFIG_DIR/es_settings.cfg)

case "$DEFE" in
"Retroarch")
	rm -rf /var/lock/start.retro
	touch /var/lock/start.retro
	systemctl start retroarch
	;;
*)
	rm /var/lock/start.games
	touch /var/lock/start.games
    systemctl start emustation
	;;
esac

source /storage/.config/custom_start.sh
