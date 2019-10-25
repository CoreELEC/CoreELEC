#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)


source /emuelec/scripts/env.sh
rp_registerAllModules

joy2keyStart

function restart_confirm() {
     if dialog --ascii-lines --yesno "ScummVM scan completed, any found games will appear next time you restart Emulationstation, do you want to restart it now?"  22 76 >/dev/tty; then
		systemctl restart emustation
      fi
 }

bash /usr/bin/scummvm.start add
bash /usr/bin/scummvm.start create
restart_confirm
