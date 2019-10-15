#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

#platform/game.shaders=/emuelec/shaders/shaders_glsl/mysnesshader.gplsp
#platform/game.ratio=16/9
#platform/game.smooth=0
#platform/game.rewind=1
#platform/game.autosave=0
#platform/game.integerscale=0
# where 0: 4:3 1: 16:9 2: 16:10 3: 16:15 4: 1:1 5: 2:1 6: 3:2 7: 3:4 8: 4:1 9: 4:4 10: 5:4 11: 6:5 12: 7:9 13: 8:3 14: 8:7 15: 19:12 16: 19:14 17: 30:17 18: 32:9 19: config (video_aspect_ratio setting) 20: 10:9 (1:1 PAR) 21: Core Provided 22: Custom
# /usr/bin/retroarch -v -L /tmp/cores/gambatte_libretro.so --config /tmp/retroarch/ee_retroarch.cfg --set-shader /tmp/shaders/crt/zfast-crt.glslp "/storage/roms/gb/[USA]/Double Dragon.zip"
# TODO: Set Atari800 to Atari5200 when neeeded / done?
# TODO: retroachivements
# TODO: Auto save/load

RETROARCHIVEMENTS=(snes nes gba gb gbc megadrive mastersystem pcengine lynx ngp atari2600 virtualboy neogeo neogeocd)
NOREWIND=(sega32x psx zxspectrum odyssey2 mame n64 dreamcast atomiswave naomi neogeocd saturn)
INDEXRATIOS=(4/3 16/9 16/10 16/15 21/9 1/1 2/1 3/2 3/4 4/1 4/4 5/4 6/5 7/9 8/3 8/7 19/12 19/14 30/17 32/9 config squarepixel core custom)
CONF="/storage/.config/emuelec/configs/emuelec.conf"
RACONF="/storage/.config/retroarch/retroarch.cfg"
RATMPCONF="/tmp/retroarch/ee_retroarch.cfg"
ORGRACORECONF="/storage/.config/retroarch/retroarch-core-options.cfg"
RACORECONF="/tmp/retroarch/retroarch-core-options.cfg"
PLATFORM=${1,,}
CORE=${3,,}
ROM="${2##*/}"
ROMEXT="${ROM##*.}"
ROM="${ROM%.*}"
SETF=0
SHADERSET=0
# Copy cfg file to tmp and remove the settings we can change
mkdir -p "/tmp/retroarch"
rm -r ${RATMPCONF}
cp ${RACONF} ${RATMPCONF}
rm ${RACORECONF}
cp ${ORGRACORECONF} ${RACORECONF}

function clean_settings() {
sed -i '/video_scale_integer =/d' $RATMPCONF
sed -i '/video_shader =/d' $RATMPCONF
sed -i '/video_shader_enable =/d' $RATMPCONF
sed -i '/video_smooth =/d' $RATMPCONF
sed -i '/video_shader =/d' $RATMPCONF
sed -i '/aspect_ratio_index =/d' $RATMPCONF
sed -i '/rewind_enable =/d' $RATMPCONF
sed -i '/run_ahead_enabled =/d' $RATMPCONF
sed -i '/run_ahead_frames =/d' $RATMPCONF
sed -i '/run_ahead_secondary_instance =/d' $RATMPCONF
}

function default_settings() {
echo 'video_scale_integer = "false"' >> $RATMPCONF
echo 'video_shader_enable = "false"' >> $RATMPCONF
echo 'video_shader = ""' >> $RATMPCONF
echo 'video_smooth = "false"'  >> $RATMPCONF 
echo 'rewind_enable = "false"'  >> $RATMPCONF 
echo 'aspect_ratio_index = "21"' >> $RATMPCONF
echo 'run_ahead_enabled = "false"' >> $RATMPCONF
echo 'run_ahead_frames = "1"' >> $RATMPCONF
echo 'run_ahead_secondary_instance = "false"' >> $RATMPCONF
}

function set_setting() {
# we set the setting on the tmp configuration
case $1 in
	"ratio")
if [[ "${2}" == "false" ]]; then
	echo 'aspect_ratio_index = "21"' >> $RATMPCONF
else	
for i in "${!INDEXRATIOS[@]}"; do
   if [[ "${INDEXRATIOS[$i]}" = "${2}" ]]; then
       break
   fi
done
	echo "aspect_ratio_index = \"${i}\""  >> $RATMPCONF
fi
	  ;;
	"smooth")
[ "$2" == "1" ] && echo 'video_smooth = "true"'  >> $RATMPCONF || echo 'video_smooth = "false"'  >> $RATMPCONF 
	  ;;
	"rewind")
(for e in "${NOREWIND[@]}"; do [[ "$e" == "$1" ]] && exit 0; done) && break
[ "$2" == "1" ] && echo 'rewind_enable = "true"' >> $RATMPCONF || echo 'rewind_enable = "false"'  >> $RATMPCONF 
	  ;;
	"autosave")
	echo "autosave = $2"  >> $RATMPCONF
	  ;;
	"integerscale")
[ "$2" == "1" ] && echo 'video_scale_integer = "true"'  >> $RATMPCONF || echo 'video_scale_integer = "false"'  >> $RATMPCONF 
	  ;;
	"shaderset")
if [ "${2}" == "false" ] || [ "${2}" == "none" ] || [ "${2}" == "0" ]; then 
	echo 'video_shader_enable = "false"' >> $RATMPCONF
	echo 'video_shader = ""' >> $RATMPCONF
else
	echo "video_shader = \"$2\"" >> $RATMPCONF
	echo 'video_shader_enable = "true"' >> $RATMPCONF
	echo "--set-shader /tmp/shaders/$2"
fi
	  ;;
	"runahead")
	if [ "${2}" == "false" ] || [ "${2}" == "none" ] || [ "${2}" == "0" ]; then 
	echo 'run_ahead_enabled = "false"' >> $RATMPCONF
	echo 'run_ahead_frames = "1"' >> $RATMPCONF
else
	echo 'run_ahead_enabled = "true"' >> $RATMPCONF
	echo "run_ahead_frames = \"${2}\"" >> $RATMPCONF
fi
	  ;;
	"secondinstance")
[ "$2" == "1" ] && echo 'run_ahead_secondary_instance = "true"' >> $RATMPCONF || echo 'run_ahead_secondary_instance = "false"'  >> $RATMPCONF 
	  ;;
esac
}

function get_setting() {
#We look for the setting on the ROM first, if not found we search for platform and lastly we search globally
	PAT="s|^${ROM}.*$1=\(.*\)|\1|p"
	EES=$(sed -n "$PAT" "$CONF")

if [ -z "$EES" ]; then
	PAT="s|^${PLATFORM}.*$1=\(.*\)|\1|p"
	EES=$(sed -n "$PAT" "$CONF")
fi

if [ -z "$EES" ]; then
	PAT="s|^global.*$1=\(.*\)|\1|p"
	EES=$(sed -n "$PAT" "$CONF")
fi

#echo set_setting $1 $EES
[ -z "$EES" ] && EES="false"
set_setting $1 $EES
}

clean_settings

get_setting "ratio"
[ -z "$EES" ] || SETF=1

get_setting "smooth"
[ -z "$EES" ] || SETF=1

get_setting "shaderset"
[ -z "$EES" ] || SETF=1

get_setting "rewind"
[ -z "$EES" ] || SETF=1

get_setting "autosave"
[ -z "$EES" ] || SETF=1

get_setting "integerscale"
[ -z "$EES" ] || SETF=1

get_setting "runahead"
[ -z "$EES" ] || SETF=1

get_setting "secondinstance"
[ -z "$EES" ] || SETF=1

if [ $SETF == 0 ]; then
clean_settings
default_settings
fi

if [ "${CORE}" == "atari800" ]; then
ATARICONF="/storage/.atari800.cfg"
sed -i "/atari800_system =/d" ${RACORECONF}
sed -i "/RAM_SIZE=/d" ${ATARICONF}
sed -i "/STEREO_POKEY=/d" ${ATARICONF}
sed -i "/BUILTIN_BASIC=/d" ${ATARICONF}


	if [ "${PLATFORM}" == "atari5200" ]; then
			echo "atari800_system = \"5200\"" >> ${RACORECONF}
            echo "RAM_SIZE=16" >> ${ATARICONF}
            echo "STEREO_POKEY=0" >> ${ATARICONF}
            echo "BUILTIN_BASIC=0" >> ${ATARICONF}
	else
			echo "atari800_system = \"800XL (64K)\"" >> ${RACORECONF}
            echo "RAM_SIZE=64" >> ${ATARICONF}
            echo "STEREO_POKEY=1" >> ${ATARICONF}
            echo "BUILTIN_BASIC=1" >> ${ATARICONF}
 	fi
fi

if [ "${CORE}" == "gambatte" ]; then
sed -i "/gambatte_gb_colorization =/d" ${RACORECONF}
sed -i "/gambatte_gb_internal_palette =/d" ${RACORECONF}

	if [ "${PLATFORM}" == "gb" ]; then
		get_setting "-renderer.colorization"
		if [ "${EES}" == "false" ] || [ "${EES}" == "auto" ] || [ "${EES}" == "none" ]; then
			echo "gambatte_gb_colorization = \"disabled\"" >> ${RACORECONF}
			echo "gambatte_gb_internal_palette = \"\"" >> ${RACORECONF}
		else
			echo "gambatte_gb_colorization = \"auto\"" >> ${RACORECONF}
			echo "gambatte_gb_internal_palette = \"${EES}\"" >> ${RACORECONF}
		fi
	fi
fi
