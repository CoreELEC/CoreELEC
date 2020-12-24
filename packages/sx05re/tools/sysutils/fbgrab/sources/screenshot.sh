#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

SCREENDIR="/storage/roms/screenshots"
FILENAME=$(date "+%Y%m%d%H%M%S%3N")

# Make sure folder exists
mkdir -p "${SCREENDIR}"

# Take screenshot
cd "${SCREENDIR}"

if [[ "$EE_DEVICE" == "OdroidGoAdvance" || "$EE_DEVICE" == "GameForce" ]]; then
    fbdump > "${FILENAME}.pbm"
else
    fbgrab -z 0 "${FILENAME}.png"
fi


