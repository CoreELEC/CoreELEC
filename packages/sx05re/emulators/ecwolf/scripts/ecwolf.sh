#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

CONFIG_DIR="/emuelec/configs/ecwolf"
CONFIG_FILE="${CONFIG_DIR}/ecwolf.cfg"

params=" --config ${CONFIG_FILE} --savedir ${CONFIG_DIR}"

case $(oga_ver) in
    "OGS")
        params+=" --res 854 480 --fullscreen --aspect 16:9"
    ;;
    "OGA")
        params+=" --res 480 320 --fullscreen --aspect 3:2"
    ;;
    "GF")
        params+=" --res 640 480 --fullscreen --aspect 4:3"
    ;;
    *)
        params+=" --fullscreen --aspect 16:9"
    ;;
esac

# data can be SD2 SD3 SOD WL6 or N3D and it's passed as the ROM
DATA=${1#*.}

# If its a mod (extension .ecwolf) read the file and parse the data
if [ ${DATA} == "ecwolf" ]; then
    while IFS== read -r key value; do
        if [ "$key" == "DATA" ]; then
            params+=" --data $value"
        fi

        if [ "$key" == "PK3" ]; then
            params+=" --file $value"
        fi
    done < "${1}"
else
    params+=" --data ${DATA}"
fi

cd "${CONFIG_DIR}"
ecwolf ${params} > /emuelec/logs/emuelec.log 2>&1 
