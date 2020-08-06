#!/bin/bash

# Source predefined functions and variables
. /etc/profile

ee_check_bios "Dinothawr" 

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/dinothawr/dinothawr.game" -Pports "${2}" -Cdinothawr "-SC${0}"
