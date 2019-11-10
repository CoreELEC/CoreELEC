#!/bin/bash

# Original script taken from here https://github.com/crcerror/RetroPie-Shares/blob/master/BGM_vol_fade.sh

BGMPATH="/storage/roms/BGM/*.mp3"

# Setup Musicplayer and Channel you want to change volume here
readonly VOLUMECHANNEL="PCM"
readonly MUSICPLAYER="mpg123"

# Get ALSA volume value and calculate step
VOLUMEALSA=$(amixer -M get $VOLUMECHANNEL | grep -o "...%]")
VOLUMEALSA=$(echo $VOLUMEALSA | sed 's/[^0-9 ]*//g')
VOLUMEALSA=$(echo $VOLUMEALSA | head -n1 | cut -d " " -f1)
FADEVOLUME=
VOLUMESTEP=

# ALSA-Commands
VOLUMEZERO="amixer -q -M set $VOLUMECHANNEL 0%"
VOLUMERESET="amixer -q -M set $VOLUMECHANNEL $VOLUMEALSA%"

function set_step() {
    case $FADEVOLUME in
        [1-4][0-9]|50) VOLUMESTEP=5 ;;
        [5-7][0-9]|80) VOLUMESTEP=3 ;;
        [8-9][0-9]|100) VOLUMESTEP=1 ;;
        *) VOLUMESTEP=5 ;;
     esac
}

function fade_out() {
    # Fading out and stop music
    FADEVOLUME=$VOLUMEALSA
    until [[ $FADEVOLUME -le 10 ]]; do
        set_step
        FADEVOLUME=$(($FADEVOLUME-$VOLUMESTEP))
        amixer -q -M set "$VOLUMECHANNEL" "${VOLUMESTEP}%-"
        sleep 0.1
    done

    $VOLUMEZERO
    echo "Killing BGM"
    killall $MUSICPLAYER
    $VOLUMERESET
}

function fade_in() {
    # Start music and fading in
    $VOLUMEZERO
    systemd-run $MUSICPLAYER -r 32000 -Z $BGMPATH
    FADEVOLUME=10
    until [[ $FADEVOLUME -ge $VOLUMEALSA ]]; do
        set_step
        FADEVOLUME=$(($FADEVOLUME+$VOLUMESTEP))
        amixer -q -M set "$VOLUMECHANNEL" "${VOLUMESTEP}%+"
        sleep 0.1
    done
    $VOLUMERESET
} 
(
case $1 in
"start")
 CFG="/storage/.emulationstation/es_settings.cfg"
 DEFE=$(sed -n 's|\s*<bool name="BGM" value="\(.*\)" />|\1|p' $CFG)
 if [ "$DEFE" == "true" ]; then
	killall -9 $MUSICPLAYER
    fade_in
fi
;;
*)
	if pgrep $MUSICPLAYER >/dev/null; then
		killall -9 $MUSICPLAYER
	fi
;;
esac
)&
