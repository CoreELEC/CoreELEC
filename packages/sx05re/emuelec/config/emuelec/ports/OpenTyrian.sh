#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

ee_check_bios "OpenTyrian"

PORT="opentyrian"

# init_port binary audio(alsa. pulseaudio, default)
init_port ${PORT} alsa

## on "Amlogic" project we need to remove asound.conf or else OpenTyrian will have no sound.
[[ "$EE_DEVICE" == "Amlogic" ]] && mv /storage/.config/asound.conf /storage/.config/asound2.conf
${PORT} -t /storage/roms/ports/opentyrian
ereturn=$?
[[ "$EE_DEVICE" == "Amlogic" ]] && mv /storage/.config/asound2.conf /storage/.config/asound.conf

end_port

exit $ereturn
