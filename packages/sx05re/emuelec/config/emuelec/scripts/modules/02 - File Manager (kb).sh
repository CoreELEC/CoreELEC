#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

if [[ "$EE_DEVICE" == "OdroidGoAdvance" ]]; then
    DinguxCommander
else
    source /emuelec/scripts/env.sh
    joy2keyStart
    mc -a
fi
