#!/bin/bash

# Source predefined functions and variables
. /etc/profile

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/doom2/doom2.wad" -Pports "${2}" -Cprboom "-SC${0}"
ret_error=$?

if [[ "$ret_error" != 0 ]]; then
	ee_check_bios "Doom2"
	exit $ret_error
else
	exit 0
fi
