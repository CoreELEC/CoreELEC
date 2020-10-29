#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

[ -z "$SYSTEM_ROOT" ] && SYSTEM_ROOT=""

EE_VERSION1=$(cat /storage/.config/EE_VERSION)
EE_VERSION2=$(cat $SYSTEM_ROOT/usr/config/EE_VERSION)

if [ "$EE_VERSION1" != "$EE_VERSION2" ]; then
    find /storage -mindepth 1 \( ! -regex '^/storage/.config/emulationstation/themes.*' -a ! -regex '^/storage/.update.*' -a ! -regex '^/storage/download.*' -a ! -regex '^/storage/roms.*' \) -delete > /dev/null 2>&1
    sync
fi
