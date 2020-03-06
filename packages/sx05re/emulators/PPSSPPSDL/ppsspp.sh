#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

if [ "$EE_DEVICE" == "OdroidGoAdvance" ]; then
echo performance > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/governor
echo performance > /sys/devices/platform/dmc/devfreq/dmc/governor
echo performance > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
fi

ARG=${1//[\\]/}
export SDL_AUDIODRIVER=alsa          
PPSSPPSDL --fullscreen "$ARG"

if [ "$EE_DEVICE" == "OdroidGoAdvance" ]; then
echo ondemand > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/governor
echo ondemand > /sys/devices/platform/dmc/devfreq/dmc/governor
echo ondemand > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
fi
