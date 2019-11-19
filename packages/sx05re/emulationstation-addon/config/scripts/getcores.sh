#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# WARNING: This file gets overwritten when the version of EmuELEC changes
# If you need to add/modify something on this file please use /storage/.emulationstation/scripts/getcores_override.sh

case "$1" in
"hatari")
	CORES="Libretro_hatari,HATARISA"
	;;
"fbn")
	CORES="Libretro_fbneo,Libretro_mame2003_plus"
	;;
"arcade")
	CORES="Libretro_mame2003_plus,AdvanceMame,Libretro_mame2010,Libretro_fbneo,Libretro_mba_mini"
	;;
"mame")
	CORES="AdvanceMame,Libretro_mame2003_plus,Libretro_mame2010,Libretro_fbneo,Libretro_mba_mini"
	;;
"psp")
	CORES="Libretro_ppsspp,PPSSPPSA"
	;;
"n64")
	CORES="Libretro_mupen64plus_next,Libretro_mupen64plus,Libretro_parallel_n64"
	;;
"nes")
	CORES="Libretro_nestopia,Libretro_fceumm"
	;;
"snes")
	CORES="Libretro_snes9x,Libretro_snes9x2002,Libretro_snes9x2005_plus"
	;;
"genesis")
	CORES="Libretro_genesis_plus_gx,Libretro_picodrive"
	;;
"sms")
	CORES="Libretro_gearsystem,Libretro_genesis_plus_gx,Libretro_picodrive"
	;;
"gba")
	CORES="Libretro_mgba,Libretro_gpsp,Libretro_vbam,Libretro_vba_next"
	;;
"gbc")
	CORES="Libretro_gambatte,Libretro_gearboy,Libretro_tgbdual"
	;;
"amiga")
	CORES="AMIBERRY,Libretro_puae,Libretro_uae4arm"
	;;
"dosbox")
	CORES="Libretro_dosbox_svn"
	;;
"dreamcast")
	CORES="Libretro_flycast,REICASTSA,REICASTSA_OLD"
	;;
"scummvm")
	CORES="Libretro_scummvm"
	;;
"neocd")
	CORES="Libretro_libneocd,Libretro_fbneo"
	;;
esac

source /storage/.emulationstation/scripts/getcores_override.sh

echo $CORES
