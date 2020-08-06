#!/bin/bash

# Source predefined functions and variables
. /etc/profile

ee_check_bios "DOOM2" 

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/doom2/doom2.wad" -Pports "${2}" -Cprboom "-SC${0}"
