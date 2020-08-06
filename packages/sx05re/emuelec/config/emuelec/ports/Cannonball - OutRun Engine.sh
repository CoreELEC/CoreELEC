#!/bin/bash

# Source predefined functions and variables
. /etc/profile

ee_check_bios "Cannonball" 

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/cannonball/" -Pports "${2}" -Ccannonball "-SC${0}"
