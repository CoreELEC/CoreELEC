#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

find / -type d \( -path /var/media -o -path /storage/roms \) -prune -o -iname *${1}* -print
