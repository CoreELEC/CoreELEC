#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

source /emuelec/scripts/env.sh
joy2keyStart

dialog --ascii-lines --max-input 32768 --msgbox "$(cat /emuelec/logs/emuelec.log)" 100 500
