#!/bin/bash

# Source predefined functions and variables
. /etc/profile

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/doom/doom1.wad" -Pports "${2}" -Cprboom "-SC${0}"
ret_error=$?

if [[ "$ret_error" != 0 ]]; then
	ee_check_bios "Doom1"
	exit $ret_error
else
	exit 0
fi
