#!/bin/bash

# Source predefined functions and variables
. /etc/profile

ee_check_bios "DOOM1" 

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/doom/doom1.wad" -Pports "${2}" -Cprboom "-SC${0}"
