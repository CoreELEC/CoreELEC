#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

source /emuelec/scripts/env.sh
source "$scriptdir/scriptmodules/supplementary/esthemes.sh"
rp_registerAllModules

joy2keyStart

function update_confirm() {
     if dialog --ascii-lines --yesno "This will kill Emulationstation and will force copy the core EmuELEC scripts. do you want to continue?"  22 76 >/dev/tty; then
		start_update
      fi
 }

function start_update() {
echo "1.0" > /storage/.config/EE_VERSION
systemd-run bash /usr/config/emuelec/scripts/force_update.sh reboot
systemctl stop emustation
}

update_confirm
