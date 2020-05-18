#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

PORT="${1}"

init_port ${PORT} alsa

case ${PORT} in
	"ffplay")
MODE=`cat /sys/class/display/mode`;	
	case "$MODE" in
		480*)
			SIZE=" -x 800 -y 480"
		;;
		576*)
			SIZE=" -x 768 -y 576"
		;;
		720*)
			SIZE=" -x 1280 -y 720"
		;;
		*)
			SIZE=" -x 1920 -y 1080"
		;;
	esac
	/usr/bin/ffplay -fs -autoexit $SIZE "${2}" > /dev/null 2>&1
	;;
	"vlc")
	# does not work...
	/usr/bin/vlc -I "dummy" --aout=alsa "${2}" vlc://quit < /dev/tty1 > /dev/null 2>&1
	;;
	"mpv")
	/usr/bin/mpv -fs "${2}" > /dev/null 2>&1
	;;
esac

end_port
