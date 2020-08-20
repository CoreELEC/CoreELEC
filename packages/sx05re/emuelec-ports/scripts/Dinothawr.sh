#!/bin/bash

# Source predefined functions and variables
. /etc/profile

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/dinothawr/dinothawr.game" -Pports "${2}" -Cdinothawr "-SC${0}"
ret_error=$?

[[ "$ret_error" != 0 ]] && ee_check_bios "Dinothawr"
exit $ret_error
