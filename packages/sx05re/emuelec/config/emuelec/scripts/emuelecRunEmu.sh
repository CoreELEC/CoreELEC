#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# This whole file has become very hacky, I am sure there is a better way to do all of this, but for now, this works.

arguments="$@"

# set audio to alsa
/storage/.config/emuelec/scripts/rr_audio.sh alsa

# Set the variables
CFG="/storage/.emulationstation/es_settings.cfg"
LOGEMU="No"
VERBOSE=""
LOGSDIR="/emuelec/logs"
EMUELECLOG="$LOGSDIR/emuelec.log"
PAT="s|\s*<string name=\"EmuELEC_$1_CORE\" value=\"\(.*\)\" />|\1|p"
EMU=$(sed -n "${PAT}" "${CFG}")
TBASH="/usr/bin/bash"
JSLISTENCONF="/emuelec/configs/jslisten.cfg"

set_kill_keys() {
	KILLTHIS=${1}
    sed -i '/program=.*/d' ${JSLISTENCONF}
	echo "program=\"/usr/bin/killall ${1}\"" >> ${JSLISTENCONF}
	}

# TEMP: I need to figure out how to mix sounds, but for now make sure BGM is killed completely to free up the soundcard
if [[ $arguments != *"KEEPMUSIC"* ]]; then
	if  pgrep mpg123 >/dev/null ; then
   ${TBASH} /storage/.emulationstation/scripts/bgm.sh stop
	fi
fi

# Make sure the /emuelec/logs directory exists
if [[ ! -d "$LOGSDIR" ]]; then
mkdir -p "$LOGSDIR"
fi


# Extract the platform from the arguments in order to show the correct bezel/splash
if [[ "$arguments" == *"-P"* ]]; then
	PLATFORM="${arguments##*-P}"  # read from -P onwards
	PLATFORM="${PLATFORM%% *}"  # until a space is found
else
# if no -P was set, read the first argument as platform
	PLATFORM="$1"
fi

[ "$1" = "LIBRETRO" ] && ROMNAME="$3" || ROMNAME="$2"

# Evkill setup
/storage/.emulationstation/scripts/configscripts/z_getkillkeys.sh
. ${JSLISTENCONF}

KILLDEV=${ee_evdev}
KILLTHIS="none"

# remove Libretro_ from the core name
EMU=$(echo "$EMU" | sed "s|Libretro_||")

# if there wasn't a --NOLOG included in the arguments, enable the emulator log output. TODO: this should be handled in ES menu
if [[ $arguments != *"--NOLOG"* ]]; then
LOGEMU="Yes"
VERBOSE="-v"
fi

# if the emulator is in es_settings this is the line that will run 
RUNTHIS='/usr/bin/retroarch $VERBOSE -L /tmp/cores/${EMU}_libretro.so "${ROMNAME}"'

# very WIP {

PAT="s|\s*<bool name=\"EmuELEC_BEZELS\" value=\"\(.*\)\" />|\1|p"
BEZ=$(sed -n "$PAT" "$CFG")
[ "$BEZ" == "true" ] && SHOW_BEZELS="Yes" || SHOW_BEZELS="No"
PAT="s|\s*<bool name=\"EmuELEC_SPLASH\" value=\"\(.*\)\" />|\1|p"
SPL=$(sed -n "$PAT" "$CFG")
[ "$SPL" == "true" ] && SHOW_SPLASH="Yes" || SHOW_SPLASH="No"

[ "$SHOW_BEZELS" = "Yes" ] && ${TBASH} /emuelec/scripts/bezels.sh "$PLATFORM" "${ROMNAME}" || ${TBASH} /emuelec/scripts/bezels.sh "default"
[ "$SHOW_SPLASH" = "Yes" ] && ${TBASH} /emuelec/scripts/show_splash.sh "$PLATFORM" "${ROMNAME}" || ${TBASH} /emuelec/scripts/show_splash.sh "default" 

# } very WIP 

# Read the first argument in order to set the right emulator
case $1 in
"HATARI")
	if [ "$EMU" = "HATARISA" ]; then
	set_kill_keys "hatari"
	RUNTHIS='${TBASH} /usr/bin/hatari.start "${ROMNAME}"'
	fi
	;;
"OPENBOR")
	set_kill_keys "OpenBOR"
	RUNTHIS='${TBASH} /usr/bin/openbor.sh "${ROMNAME}"'
	;;
"RETROPIE")
	RUNTHIS='${TBASH} /emuelec/scripts/fbterm.sh "${ROMNAME}"'
	EMUELECLOG="$LOGSDIR/ee_script.log"
	;;
"LIBRETRO")
	RUNTHIS='/usr/bin/retroarch $VERBOSE -L /tmp/cores/$2_libretro.so "${ROMNAME}"'
		;;
"REICAST")
    if [ "$EMU" = "REICASTSA" ]; then
    set_kill_keys "reicast"
	RUNTHIS='${TBASH} /usr/bin/reicast.sh "${ROMNAME}"'
	LOGEMU="No" # ReicastSA outputs a LOT of text, only enable for debugging.
	fi
	;;
"MAME"|"ARCADE")
	if [ "$EMU" = "AdvanceMame" ]; then
	set_kill_keys "advmame"
	RUNTHIS='${TBASH} /usr/bin/advmame.sh "${ROMNAME}"'
	fi
	;;
"DRASTIC")
	set_kill_keys "drastic"
	RUNTHIS='${TBASH} /storage/.emulationstation/scripts/drastic.sh "${ROMNAME}"'
		;;
"N64")
    if [ "$EMU" = "M64P" ]; then
    set_kill_keys "mupen64plus"
	RUNTHIS='${TBASH} /usr/bin/m64p.sh "${ROMNAME}"'
	fi
	;;
"AMIGA")
    if [ "$EMU" = "AMIBERRY" ]; then
    set_kill_keys "amiberry"
	RUNTHIS='${TBASH} /usr/bin/amiberry.start "${ROMNAME}"'
	fi
	;;
"SCUMMVM")
    if [ "$EMU" = "SCUMMVMSA" ]; then
    set_kill_keys "scummvm"
	RUNTHIS='${TBASH} /usr/bin/scummvm.start sa'
	else
	RUNTHIS='${TBASH} /usr/bin/scummvm.start libretro'
	fi
	;;
"DOSBOX")
    if [ "$EMU" = "DOSBOXSDL2" ]; then
    set_kill_keys "dosbox"
	RUNTHIS='${TBASH} /usr/bin/dosbox.start "${ROMNAME}"'
	fi
	;;		
"PSP")
	if [ "$EMU" = "PPSSPPSA" ]; then
	#PPSSPP can run at 32BPP but only with buffered rendering, some games need non-buffered and the only way they work is if I set it to 16BPP
	# /emuelec/scripts/setres.sh 16 # This was only needed for S912, but PPSSPP does not work on S912 
	RUNTHIS='${TBASH} /usr/bin/ppsspp.sh "${ROMNAME}"'
	fi
	;;
"NEOCD")
	if [ "$EMU" = "fbneo" ]; then
	RUNTHIS='/usr/bin/retroarch $VERBOSE -L /tmp/cores/fbneo_libretro.so --subsystem neocd "${ROMNAME}"'
	fi
	;;
esac

# Clear the log file
echo "EmuELEC Run Log" > $EMUELECLOG

# Write the command to the log file.
echo "PLATFORM: $PLATFORM" >> $EMUELECLOG
echo "ROM NAME: ${ROMNAME}" >> $EMUELECLOG
echo "1st Argument: $1" >> $EMUELECLOG 
echo "2nd Argument: $2" >> $EMUELECLOG
echo "3rd Argument: $3" >> $EMUELECLOG 
echo "4th Argument: $4" >> $EMUELECLOG 
echo "Run Command is:" >> $EMUELECLOG 
eval echo  ${RUNTHIS} >> $EMUELECLOG 

# TEMP: I need to figure out how to mix sounds, but for now make sure BGM is killed completely to free up the soundcard
if [[ $arguments != *"KEEPMUSIC"* ]]; then
	killall -9 mpg123 
fi

if [[ "$KILLTHIS" != "none" ]]; then
	/emuelec/bin/jslisten --device /dev/input/${KILLDEV} &
fi

# Exceute the command and try to output the results to the log file if it was not dissabled.
if [[ $LOGEMU == "Yes" ]]; then
   echo "Emulator Output is:" >> $EMUELECLOG
   eval ${RUNTHIS} >> $EMUELECLOG 2>&1
else
   echo "Emulator log was dissabled" >> $EMUELECLOG
   eval ${RUNTHIS}
fi 

# Only run resetfb if it exists, mainly for N2
if [ -f "/emuelec/scripts/resetfb.sh" ]; then
${TBASH} /emuelec/scripts/resetfb.sh
fi

if [ ! -e /proc/device-tree/t82x@d00c0000/compatible ]; then
# Yet even more hacks to get S912 to play nice, don't display a splash on S912 after quiting a game
${TBASH} /emuelec/scripts/show_splash.sh intro
fi

if [[ $arguments != *"KEEPMUSIC"* ]]; then
	DEFE=$(sed -n 's|\s*<bool name="BGM" value="\(.*\)" />|\1|p' $CFG)
 if [ "$DEFE" == "true" ]; then
	killall -9 mpg123
	${TBASH} /storage/.emulationstation/scripts/bgm.sh start
 fi 
fi

# Kill jslisten, we don't need to but just to make sure 
killall jslisten

# Just for good measure lets make a symlink to Retroarch logs if it exists
if [[ -f "/storage/.config/retroarch/retroarch.log" ]]; then
	ln -sf /storage/.config/retroarch/retroarch.log ${LOGSDIR}/retroarch.log
fi

#{log_addon}#

# Return to default mode
${TBASH} /emuelec/scripts/setres.sh

# reset audio to pulseaudio
/storage/.config/emuelec/scripts/rr_audio.sh pulseaudio
