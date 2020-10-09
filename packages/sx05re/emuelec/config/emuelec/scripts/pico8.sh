#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# If there is a new version copy the files
if [[ -e "/storage/roms/bios/pico-8" ]]; then
    cp -rf /storage/roms/bios/pico-8 /emuelec/bin/
    rm -rf /storage/roms/bios/pico-8
    chmod +x /emuelec/bin/pico-8/pico8_dyn
    touch /storage/roms/pico-8/splore.p8
fi 

mkdir -p /emuelec/configs/pico-8

CART=name=$(echo "${1}" | cut -f 1 -d '.')

if [[ "${CART}" == *"/splore"* ]]; then
    /emuelec/bin/pico-8/pico8_dyn -splore -home /emuelec/configs/pico-8 -root_path /storage/roms/pico-8 -joystick 0
else
    /emuelec/bin/pico-8/pico8_dyn -run ${CART} -home /emuelec/configs/pico-8 -root_path /storage/roms/pico-8 -joystick 0
fi
