#!/bin/bash

# Source predefined functions and variables
. /etc/profile

ee_check_bios "CaveStory" 

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/CaveStory/Doukutsu.exe" -Pports "${2}" -Cnxengine "-SC${0}"
