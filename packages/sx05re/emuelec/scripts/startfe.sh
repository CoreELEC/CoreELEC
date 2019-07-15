#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

    touch /var/lock/start.games
    systemctl start emustation
    killkodi.sh > /dev/null 2>&1
