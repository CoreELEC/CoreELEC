#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present SumavisionQ5 (https://github.com/SumavisionQ5)
# Modifications by Shanti Gilbert (https://github.com/shantigilbert)

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

if [ "$PLATFORM" == "intro" ]; then
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

if [[ -f "/storage/.config/emuelec/configs/novideo" ]]; then
(
if [ ! -e /proc/device-tree/t82x@d00c0000/compatible ] || [ -f "/emuelec/bin/fbfix" ]; then
	mpv $SPLASH > /dev/null 2>&1
  else
    fbi $SPLASH t 1 -noverbose > /dev/null 2>&1
fi 
)&
else
	SPLASH="/storage/.config/splash/emuelec_intro_1080p.mp4"
	mpv $SPLASH > /dev/null 2>&1
	touch "/storage/.config/emuelec/configs/novideo"
fi
