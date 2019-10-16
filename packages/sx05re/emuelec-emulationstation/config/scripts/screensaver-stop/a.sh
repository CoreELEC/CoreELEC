#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

DEFE=$(sed -n 's|\s*<bool name="BGM" value="\(.*\)" />|\1|p' /storage/.emulationstation/es_settings.cfg)

	if [ "$DEFE" == "true" ]; then
	killall mpg123
	/storage/.emulationstation/scripts/bgm.sh start
	fi 
