#!/usr/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

CONFIG="/emuelec/configs/chocolate-doom"
LOCALCONFIG="/storage/.local/share/chocolate-doom"

if [ ! -L "${LOCALCONFIG}" ]; then 
[[ -d "${LOCALCONFIG}" ]] && rm -rf "${LOCALCONFIG}"
ln -sf "${CONFIG}" "${LOCALCONFIG}"
fi

GUID=$(echo "${2}" | grep -Ewo '[[:xdigit:]]{32}' | head -n1)
# echo "Will use controller with ${GUID}"
# Try to set up gamepad
sed -i "s|joystick_guid.*|joystick_guid                 \"${GUID}\"|" "${CONFIG}/chocolate-doom.cfg"

/usr/bin/chocolate-doom -iwad "${1}"


