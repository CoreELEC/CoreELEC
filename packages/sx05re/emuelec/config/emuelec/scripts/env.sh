#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

export TERM="linux"
export TERMINFO="/usr/share/terminfo"
export TERMINFO_DIRS=$TERMINFO
export HOME="/storage"
export romdir="/storage/roms/"
export configdir="/storage/.config/emuelec/configs"
export scriptdir="/emuelec"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/emuelec/lib
export PATH=$PATH:/emuelec/bin:/emuelec/scripts

user="root"
rootdir="/emuelec"
home="/storage"
datadir="$home/.config/emuelec/RetroPie"
romdir="$home/roms"
biosdir="$romdir/BIOS"
emudir="$rootdir/emulators"
configdir="$rootdir/configs"
__logdir="$scriptdir/logs"
__tmpdir="/tmp"
__builddir="$__tmpdir/build"
__swapdir="$__tmpdir"
__backtitle="EmuELEC - RetroPie configuration"

source "$scriptdir/scriptmodules/system.sh"
source "$scriptdir/scriptmodules/helpers.sh"
source "$scriptdir/scriptmodules/inifuncs.sh"
source "$scriptdir/scriptmodules/packages.sh"

# hack/workaround for the S912
if [ -e /proc/device-tree/t82x@d00c0000/compatible ]; then
	/emuelec/scripts/setres.sh 16
	for i in {1..10}; do cat /etc/motd; done
else 
cat /etc/motd
echo "Loading...Please Wait!"
	if [ -f "/emuelec/bin/fbfix" ]; then
		/emuelec/bin/fbfix
	fi
fi
