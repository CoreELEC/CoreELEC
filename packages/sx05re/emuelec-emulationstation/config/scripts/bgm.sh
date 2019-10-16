#!/bin/bash

# not needed in batocera
exit 0

BGMPATH="/storage/roms/BGM/*.mp3"

# Setup Musicplayer and Channel you want to change volume here
readonly MUSICPLAYER="mpg123"
readonly OUTDEV="pulse"
readonly RATE="44100"
systemctl import-environment PATH
systemctl import-environment LD_LIBRARY_PATH
systemctl import-environment SDL_AUDIODRIVER
MAXVOLUME="100"

function set_step() {
    case $FADEVOLUME in
        [1-4][0-9]|50) VOLUMESTEP=10 ;;
        [5-7][0-9]|80) VOLUMESTEP=5 ;;
        [8-9][0-9]|100) VOLUMESTEP=3 ;;
        *) VOLUMESTEP=5;;
     esac
}

function set_vol() {
	pactl set-sink-input-volume $(pactl list sink-inputs | grep $MUSICPLAYER -B 20 | grep "#" | cut -d \# -f 2) "$1"%
	}

function fade_in() {
VOLUME=$MAXVOLUME
FADEVOLUME=0
    
    set_vol 0
    until [[ $FADEVOLUME -ge $VOLUME ]]; do
        set_step
        FADEVOLUME=$(($FADEVOLUME+$VOLUMESTEP))
        #echo $FADEVOLUME
        set_vol $FADEVOLUME
        sleep 0.1
    done
}	
	
function fade_out() {
FADEVOLUME=$(pactl list sink-inputs | grep "$MUSICPLAYER" -B 20 | grep "front-left:" | cut -d "/" -f 2 | cut -d \% -f 0)

	until [[ $FADEVOLUME -le 10 ]]; do
        set_step
        FADEVOLUME=$(($FADEVOLUME-$VOLUMESTEP))
        set_vol $FADEVOLUME
       # echo $FADEVOLUME
        sleep 0.1
    done
    echo "Killing BGM"
    killall $MUSICPLAYER	
}

(
if [[ "$1" = "start" ]]; then
	if ! pgrep $MUSICPLAYER >/dev/null; then
		systemd-run $MUSICPLAYER -o $OUTDEV -r $RATE -Z $BGMPATH
		sleep 0.2 # avoid error about sink not being loaded
		fade_in
	fi
else
	fade_out
fi
)&
