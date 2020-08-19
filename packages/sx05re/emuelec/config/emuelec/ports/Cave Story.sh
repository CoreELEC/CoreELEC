#!/bin/bash

# Source predefined functions and variables
. /etc/profile

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/CaveStory/Doukutsu.exe" -Pports "${2}" -Cnxengine "-SC${0}"
ret_error=$?

if [[ "$ret_error" != 0 ]]; then
	ee_check_bios "CaveStory"
	exit $ret_error
else
	exit 0
fi
