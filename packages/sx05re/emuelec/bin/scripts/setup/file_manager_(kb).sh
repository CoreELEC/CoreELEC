#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

if [[ "$EE_DEVICE" == "OdroidGoAdvance" ]] || [[ "$EE_DEVICE" == "GameForce" ]]; then
    cd /usr/bin
    DinguxCommander
else
    source /usr/bin/env.sh
    joy2keyStart
    mc -a
    joy2keyStop
fi
