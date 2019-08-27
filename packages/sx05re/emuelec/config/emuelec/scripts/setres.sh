#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Read the video output mode and set it for emuelec to avoid video flicking.
MODE=`cat /sys/class/display/mode`;

if [ -e /proc/device-tree/t82x@d00c0000/compatible ]; then
	umount /system/build.prop
	rm /storage/build.prop
fi 

case "$1" in
"16")
	BPP=16
if [ -e /proc/device-tree/t82x@d00c0000/compatible ]; then
	cp /system/build.prop /storage/build.prop
	sed -i 's/32/16/g' /storage/build.prop
	mount -o bind /storage/build.prop /system/build.prop
fi 
	;;
*)
	BPP=32
	;;
esac

if [ ! -e /proc/device-tree/t82x@d00c0000/compatible ]; then
# always set 32 BPP for S905/N2
 BPP=32
fi

#fbset -fb /dev/fb0 -g 1920 1080 1920 2160 $BPP
