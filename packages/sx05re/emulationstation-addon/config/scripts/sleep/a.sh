#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

DEFE=get_es_setting bool "BGM"

	if [ "$DEFE" == "true" ]; then
	    if  pgrep mpg123 >/dev/null ; then
	/storage/.emulationstation/scripts/bgm.sh stop
	    fi
	fi 
