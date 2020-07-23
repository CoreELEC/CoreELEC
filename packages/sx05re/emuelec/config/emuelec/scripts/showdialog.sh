#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

source /emuelec/scripts/env.sh
joy2keyStart

if [[ ! -z "${1}" ]] && [[ ! -z "${2}" ]]; then
	if [ "$EE_DEVICE" == "OdroidGoAdvance" ]; then
		dialog --backtitle 'EmuELEC Error' --title "${1} Error" --ascii-lines --colors --no-collapse --ok-label 'Close' --msgbox "${2}" 20 35
	else
		dialog --backtitle 'EmuELEC Error' --title "${1} Error" --ascii-lines --colors --no-collapse --ok-label 'Close' --msgbox "${2}" 30 80
	fi
fi
