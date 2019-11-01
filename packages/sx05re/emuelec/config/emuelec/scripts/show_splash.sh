#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present SumavisionQ5 (https://github.com/SumavisionQ5)
# Modifications by Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

PLATFORM="$1"

case $PLATFORM in
 "ARCADE"|"FBA"|"NEOGEO"|"MAME"|CPS*)
   PLATFORM="ARCADE"
  ;;
 "RETROPIE")
   # fbterm does not like the splash screen 
   exit 0
  ;;
esac

if [ "$PLATFORM" == "intro" ] || [ "$PLATFORM" == "exit" ]; then
	SPLASH="/storage/.config/splash/splash-1080.png"
elif [ "$PLATFORM" == "default" ]; then
	SPLASH="/storage/.config/splash/loading-game.png"
else
	SPLASHDIR="/storage/overlays/splash"
	ROMNAME=$(basename "${2%.*}")
	SPLMAP="/emuelec/bezels/arcademap.cfg"
	SPLNAME=$(sed -n "/`echo ""$PLATFORM"_"${ROMNAME}" = "`/p" "$SPLMAP")
	REALSPL="${SPLNAME#*\"}"
	REALSPL="${REALSPL%\"*}"
	if [[ -d "$SPLASHDIR/$PLATFORM" ]]; then
		[ ! -z "$REALSPL" ] && SPLASH1=$(find $SPLASHDIR/$PLATFORM -iname "$REALSPL*.png" -maxdepth 1 | sort -V | head -n 1)
		[ ! -z "$ROMNAME" ] && SPLASH2=$(find $SPLASHDIR/$PLATFORM -iname "$ROMNAME*.png" -maxdepth 1 | sort -V | head -n 1)
		SPLASH3="$SPLASHDIR/$PLATFORM/splash.png"
	fi	
	if [ -f "$SPLASH1" ]; then
		SPLASH=$SPLASH1
	elif [ -f "$SPLASH2" ]; then
		SPLASH=$SPLASH2
	elif [ -f "$SPLASH3" ]; then
		SPLASH=$SPLASH3
	else
		SPLASH="/storage/.config/splash/loading-game.png"
	fi
fi 

[[ "${PLATFORM}" != "intro" ]] && VIDEO=0 || VIDEO=$(get_ee_setting bootvideo.enabled)

if [[ -f "/storage/.config/emuelec/configs/novideo" ]] && [[ ${VIDEO} != "1" ]]; then
	if [ "$PLATFORM" != "intro" ]; then
	(
		if [ ! -e /proc/device-tree/t82x@d00c0000/compatible ]; then
			mpv $SPLASH > /dev/null 2>&1
		else
			ply-image $SPLASH > /dev/null 2>&1
		fi 
	)&
	fi 
else
	SPLASH="/usr/config/splash/emuelec_intro_1080p.mp4"
	mpv $SPLASH > /dev/null 2>&1
	touch "/storage/.config/emuelec/configs/novideo"
fi

# Wait for the time specified in ee_splash_delay setting in emuelec.conf
SPLASHTIME=$(get_ee_setting splash.delay)
[ "$SPLASHTIME" =~ '^[0-9]+([.][0-9]+)?$' ] && sleep $SPLASHTIME
