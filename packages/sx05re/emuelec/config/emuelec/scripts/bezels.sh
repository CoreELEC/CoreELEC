#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present SumavisionQ5 (https://github.com/SumavisionQ5)
# Modifications by Shanti Gilbert (https://github.com/shantigilbert)

PLATFORM="$1"

ROMNAME=$(basename "${2%.*}")
RACONFIG="/storage/.config/retroarch/retroarch.cfg"
OPACITY="1.000000"
AR_INDEX="23"
BEZELDIR="/tmp/overlays/bezels"
INIFILE="/emuelec/bezels/settings.ini"
DEFAULT_BEZEL="false"

case $PLATFORM in
 "ARCADE"|"FBA"|"NEOGEO"|"MAME"|CPS*)
   PLATFORM="ARCADE"
  ;;
  "default")
  if [ -f "/storage/.config/bezels_enabled" ]; then
  clear_bezel
  sed -i '/input_overlay = "/d' $RACONFIG
  rm "/storage/.config/bezels_enabled"
  fi
   exit 0
  ;;
  "RETROPIE")
  # fbterm does not need bezels
  exit 0
  ;;
esac

 if [ ! -f "/storage/.config/bezels_enabled" ]; then
   touch /storage/.config/bezels_enabled
 fi

# we make sure the platform is all lowercase
PLATFORM=${PLATFORM,,}

# if a backup does not exists make a copy of retroarch.cfg so we can return to it when we disable bezels
 # for future use...maybe
 #if [ ! -f "$RACONFIG.org" ]; then
 #  cp "$RACONFIG" "$RACONFIG.org"
 #fi

# bezelmap.cfg in $BEZELDIR/ is to share bezels between arcade clones and parent. 
BEZELMAP="/emuelec/bezels/arcademap.cfg"
BZLNAME=$(sed -n "/"$PLATFORM"_"$ROMNAME" = /p" "$BEZELMAP")
BZLNAME="${BZLNAME#*\"}"
BZLNAME="${BZLNAME%\"*}"
OVERLAYDIR1=$(find $BEZELDIR/$PLATFORM -maxdepth 1 -iname "$ROMNAME*.cfg" | sort -V | head -n 1)
[ ! -z "$BZLNAME" ] && OVERLAYDIR2=$(find $BEZELDIR/$PLATFORM -maxdepth 1 -iname "$BZLNAME*.cfg" | sort -V | head -n 1)
OVERLAYDIR3="$BEZELDIR/$PLATFORM/default.cfg"

clear_bezel() { 
		sed -i '/aspect_ratio_index = "/d' $RACONFIG
		sed -i '/custom_viewport_width = "/d' $RACONFIG
		sed -i '/custom_viewport_height = "/d' $RACONFIG
		sed -i '/custom_viewport_x = "/d' $RACONFIG
		sed -i '/custom_viewport_y = "/d' $RACONFIG
		sed -i '/video_scale_integer = "/d' $RACONFIG
		sed -i '/input_overlay_opacity = "/d' $RACONFIG
		echo 'video_scale_integer = "false"' >> $RACONFIG
		echo 'input_overlay_opacity = "0.150000"' >> $RACONFIG
		}

set_bezel() {
# $OPACITY: input_overlay_opacity
# $AR_INDEX: aspect_ratio_index
# $1: custom_viewport_width 
# $2: custom_viewport_height
# $3: ustom_viewport_x
# $4: custom_viewport_y
# $5: video_scale_integer
# $6: video_scale 
        
        clear_bezel
        sed -i '/input_overlay_opacity = "/d' $RACONFIG
        sed -i "1i input_overlay_opacity = \"$OPACITY\"" $RACONFIG
		sed -i "2i aspect_ratio_index = \"$AR_INDEX\"" $RACONFIG
		sed -i "3i custom_viewport_width = \"$1\"" $RACONFIG
		sed -i "4i custom_viewport_height = \"$2\"" $RACONFIG
		sed -i "5i custom_viewport_x = \"$3\"" $RACONFIG
		sed -i "6i custom_viewport_y = \"$4\"" $RACONFIG
		sed -i "7i video_scale_integer = \"$5\"" $RACONFIG
	}

check_overlay_dir() {

# The bezel will be searched and used in following order:
# 1.$OVERLAYDIR1 will be used, if it does not exist, then
# 2.$OVERLAYDIR2 will be used, if it does not exist, then
# 3.$OVERLAYDIR2 platform default bezel as "$BEZELDIR/"$PLATFORM"/default.cfg\" will be used.
# 4.Default bezel at "$BEZELDIR/default.cfg\" will be used.
	
	sed -i '/input_overlay = "/d' $RACONFIG
		
	if [ -f "$OVERLAYDIR1" ]; then
		echo -e "input_overlay = \""$OVERLAYDIR1"\"\n" >> $RACONFIG
	elif [ -f "$OVERLAYDIR2" ]; then
		echo -e "input_overlay = \""$OVERLAYDIR2"\"\n" >> $RACONFIG
	elif [ -f "$OVERLAYDIR3" ]; then
		echo -e "input_overlay = \""$OVERLAYDIR3"\"\n" >> $RACONFIG
	else
		echo -e "input_overlay = \"$BEZELDIR/default.cfg\"\n" >> $RACONFIG
		DEFAULT_BEZEL="true"
	fi
}

# Only 720P and 1080P can use bezels. For 480p/i and 576p/i we just delete bezel config.
hdmimode=$(cat /sys/class/display/mode)

case $hdmimode in
  480*)
	sed -i '/input_overlay = "/d' $RACONFIG
	clear_bezel
  ;;
  576*)
	sed -i '/input_overlay = "/d' $RACONFIG
	clear_bezel
  ;;
  720p*)
	
	check_overlay_dir "$PLATFORM"
	case "$PLATFORM" in
   "GBA")
		set_bezel "467" "316" "405" "190" "false"
		;;
	"GAMEGEAR")
		set_bezel "780" "580" "245" "70" "false"
		;;
	"GB")
		set_bezel "429" "380" "420" "155" "false"
		;;
	"GBC")
		set_bezel "430" "380" "425" "155" "false"
		;;
	"NGP")
		set_bezel "461" "428" "407" "145" "false"
		;;
    "NGPC")
		set_bezel "460" "428" "407" "145" "false"
		;;
	"WONDERSWAN")
		set_bezel "645" "407" "325" "150" "false"
		;;
	"WONDERSWANCOLOR")
		set_bezel "643" "405" "325" "150" "false"
		;;
	*)
		# delete aspect_ratio_index to make sure video is expanded fullscreen. Only certain handheld platforms need custom_viewport.
		clear_bezel
		sed -i '/input_overlay_opacity = "/d' $RACONFIG
        sed -i "1i input_overlay_opacity = \"$OPACITY\"" $RACONFIG
		;;
	esac
  ;;
  # For Amlogic TV box, the following resolution is 1080p/i.
  1080*)
    check_overlay_dir "$PLATFORM"
	case "$1" in
   "GBA")
		set_bezel "698" "472" "609" "288" "false"
		;;
	"GAMEGEAR")
		set_bezel "1160" "850" "380" "120" "false"
		;;
	"GB")
		set_bezel "625" "565" "645" "235" "false"
		;;
	"GBC")
		set_bezel "625" "565" "645" "235" "false"
		;;
	"NGP")
		set_bezel "700" "635" "610" "220" "false"
		;;
	"NGPC")
		set_bezel "700" "640" "610" "215" "false"
		;;
	"WONDERSWAN")
		set_bezel "950" "605" "490" "225" "false"
		;;
	"WONDERSWANCOLOR")
		set_bezel "950" "605" "490" "225" "false"
		;;
	*)
		clear_bezel
		sed -i '/input_overlay_opacity = "/d' $RACONFIG
		sed -i "1i input_overlay_opacity = \"$OPACITY\"" $RACONFIG
		;;
	esac
  ;;
esac

	if [ "$DEFAULT_BEZEL" = "true" ]; then
		set_bezel "1427" "1070" "247" "10" "false"
	fi
	    
# If we disable bezel in setting.ini for certain platform, we just delete bezel config.
Bezel=$(sed -n "/"$PLATFORM"_Bezel = /p" $INIFILE)
Bezel="${Bezel#*\"}"
Bezel="${Bezel%\"*}"
if [ "$Bezel" = "OFF" ]; then
sed -i '/input_overlay = "/d' $RACONFIG
fi

# Note:
# 1. Different handheld platforms have different bezels, they may need different viewport value even for same platform.
#	So, I think this script should be stored in $BEZELDIR/ or some place wich can be modified by users.
# 2. For Arcade games, I created a bezelmap.cfg in $BEZELDIR/ in order to share bezels between arcade clones and parent. 
#	In fact, ROMs of other platforms can share certain bezel if you write mapping relationship in bezelmap.cfg.
# 3. I modified es_systems.cfg to set $1 as platfrom for all platfrom.
#	For some libretro core such as <command>/usr/bin/sx05reRunEmu.sh LIBRETRO scummvm %ROM%</command>, $1 not right platform value,
#	you may need some tunings on them.
# 4. I am a Linux noob, so the codes are a mess. Sorry for that:)
