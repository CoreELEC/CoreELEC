#!/bin/bash

if [[ ! -f /storage/roms/ports/CaveStory/Doukutsu.exe ]] || [[ ! -d /storage/roms/ports/CaveStory/data ]]; then
    /emuelec/scripts/fbterm.sh error "Cavestory" "Could not find Doukutsu.exe or the data directory, please make sure you copied the game into /storage/roms/ports/CaveStory \n\n This box will close in 10 seconds" &
    error_process="$!"
    sleep 10
    pkill -P $error_process
    exit 1
fi

/emuelec/scripts/emuelecRunEmu.sh "/storage/roms/ports/CaveStory/Doukutsu.exe" -Pports "${2}" -Cnxengine "-SC${0}"
