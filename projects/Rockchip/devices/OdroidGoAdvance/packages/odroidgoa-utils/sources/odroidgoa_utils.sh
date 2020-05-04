#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# THESE NEEDS TO BE CLEANED UP, MAYBE WITH CASE OR FUNCTIONS

# Source predefined functions and variables
. /etc/profile

if [ "${1}" == "toggleaudio" ];then
# Toggle audio output
CURRENTAUDIO=$(get_ee_setting "audio.device")
	case "${CURRENTAUDIO}" in
	    "headphone")
	    echo "setting speakers"
		amixer cset name='Playback Path' SPK
		set_ee_setting "audio.device" "speakers"
		;;
	    "auto"|"speakers"|*)
	    echo "setting headphones"
		amixer cset name='Playback Path' HP
		set_ee_setting "audio.device" "headphone"
		;;
	esac
fi

if [ "${1}" == "setaudio" ];then
# Set audio output second parameter is either headphones or speakers
	case "${2}" in
	    "headphone")
	    echo "setting headphones"
		amixer cset name='Playback Path' HP
		set_ee_setting "audio.device" "headphone"
		;;
	  	"auto"|"speakers"|*)
	  	echo "setting speakers"
		amixer cset name='Playback Path' SPK
		set_ee_setting "audio.device" "speakers"
		;;
	esac
fi

if [ "${1}" == "vol" ];then
VOLSTEP=5
CURRENTVOL=$(get_ee_setting "audio.volume")
MAXVOL=100
MINVOL=0
	if [ "${2}" == "+" ]; then
		STEPVOL=$(($CURRENTVOL+$VOLSTEP))
	elif [ "${2}" == "-" ]; then
		STEPVOL=$(($CURRENTVOL-$VOLSTEP))
	else
		STEPVOL=${2}
	fi
	[ "$STEPVOL" -ge "$MAXVOL" ] && STEPVOL="$MAXVOL"
	[ "$STEPVOL" -le "$MINVOL" ] && STEPVOL="$MINVOL"
	amixer set 'Playback' ${STEPVOL}%
	set_ee_setting "audio.volume" ${STEPVOL}
  fi    

if [ "${1}" == "bright" ]; then
STEPS="20"
CURRENTBRIGHT=$(cat /sys/class/backlight/backlight/brightness)
MAXBRIGHT="255" #$(cat /sys/class/backlight/backlight/max_brightness)
MINBRIGHT="2"
	if [ "${2}" == "+" ]; then
		STEPBRIGHT=$(($CURRENTBRIGHT+$STEPS))
	elif [ "${2}" == "-" ]; then
		STEPBRIGHT=$(($CURRENTBRIGHT-$STEPS))
	else
		STEPBRIGHT=${2}
	fi
	[ "$STEPBRIGHT" -ge "$MAXBRIGHT" ] && STEPBRIGHT="$MAXBRIGHT"
	[ "$STEPBRIGHT" -le "$MINBRIGHT" ] && STEPBRIGHT="$MINBRIGHT"
	#echo "Setting bright to $STEPBRIGHT"
	echo "${STEPBRIGHT}" > /sys/class/backlight/backlight/brightness
	set_ee_setting "brightness.level" $(cat /sys/class/backlight/backlight/brightness)
fi

