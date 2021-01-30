#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

function restart_confirm() {
     echo -en "ScummVM scan completed, any found games will appear next time you restart Emulationstation, do you want to restart it now?" > /tmp/display
		[[ $? == 21 ]] && systemctl restart emustation || exit 0; 
      fi
 }

ee_console enable
bash /usr/bin/scummvm.start add
bash /usr/bin/scummvm.start create
restart_confirm
ee_console disable
