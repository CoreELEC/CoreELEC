#!/bin/bash

# Source predefined functions and variables
. /etc/profile

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/xrick/data.zip" -Pports "${2}" -Cxrick "-SC${0}"
ret_error=$?

[[ "$ret_error" != 0 ]] && ee_check_bios "RickDangerous"
exit $ret_error
