#!/bin/bash

# Source predefined functions and variables
. /etc/profile

ee_check_bios "REminiscence"

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/reminiscence" -Pports "${2}" -Creminiscence "-SC${0}"
