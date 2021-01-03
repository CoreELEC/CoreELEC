#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

source /emuelec/scripts/env.sh
joy2keyStart

function update_confirm() {
     dialog --ascii-lines --msgbox "THIS SCRIPT IS IS OBSOLETE, USE THE UPDATE & DOWNLOADS INSTEAD!"  22 76
 }

update_confirm
