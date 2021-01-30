#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

LOGLINK=$(emueleclogs.sh)
echo -en "Use this link to ask for help:\n\n${LOGLINK}" > /tmp/display
text_viewer -t "EmuELEC Send Logs" -f 24 /tmp/display
rm /tmp/display > /dev/null 2&>1

