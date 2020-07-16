#!/bin/bash

# Source predefined functions and variables
. /etc/profile

ee_check_bios "RickDangerous"

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/xrick/data.zip" -Pports "${2}" -Cxrick "-SC${0}"
