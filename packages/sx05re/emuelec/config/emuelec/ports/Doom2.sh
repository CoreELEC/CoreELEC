#!/bin/bash

# Source predefined functions and variables
. /etc/profile

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/doom2/doom2.wad" -Pports "${2}" -Cprboom "-SC${0}"
ret_error=$?

[[ "$ret_error" != 0 ]] && ee_check_bios "Doom2"
exit $ret_error
