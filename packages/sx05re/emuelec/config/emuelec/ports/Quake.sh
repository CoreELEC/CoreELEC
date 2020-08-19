#!/bin/bash

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/quake/id1/pak0.pak" -Pports "${2}" -Ctyrquake "-SC${0}"

if [[ "$ret_error" != 0 ]]; then
	ee_check_bios "Quake"
	exit $ret_error
else
	exit 0
fi
