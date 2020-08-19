#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

PORT="eduke32"

# init_port binary audio(alsa. pulseaudio, default)
init_port ${PORT} alsa

cd /storage/.config/SDL-GameControllerDB/
EDUKE32_MUSIC_CMD="timidity" ${PORT} -g /storage/roms/ports/eduke/duke3d.grp /storage/roms/ports/eduke/DUKE.RTS >> $EE_LOG 2>&1
ret_error=$?

end_port

[[ "$ret_error" != 0 ]] && ee_check_bios "eduke32"
exit $ret_error
