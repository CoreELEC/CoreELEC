#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Libretro cores should be added as Libretro_nameofcore"
# Stand alone emulators should also be added to emuelecRunEmu.sh to add the command line
# these platforms are the only ones that support ES selection

# case "$1" in
# "fba")
#	CORES="Libretro_fbalpha,Libretro_mame2003_plus"
#	;;
# "arcade")
#	CORES="Libretro_mame2003_plus,AdvanceMame,Libretro_mame2010,Libretro_mame2015,Libretro_fbalpha"
#	;;
# "mame")
#	CORES="AdvanceMame,Libretro_mame2003_plus,Libretro_mame2010,Libretro_mame2015,Libretro_fbalpha"
#	;;
# "psp")
#	CORES="PPSSPPSA,Libretro_ppsspp"
#	;;
#"n64")
#	CORES="Libretro_mupen64plus,Libretro_parallel_n64,Libretro_mupen64plus_next,M64P"
#	;;
#"nes")
#	CORES="Libretro_nestopia,Libretro_fceumm,Libretro_quicknes"
#	;;
#"snes")
#	CORES="Libretro_snes9x,Libretro_snes9x2002,Libretro_snes9x2005,Libretro_snes9x2005_plus,Libretro_snes9x2010"
#	;;
#"genesis")
#	CORES="Libretro_genesis_plus_gx,Libretro_picodrive"
#	;;
#"gba")
#	CORES="Libretro_mgba,Libretro_gpsp,Libretro_vbam,Libretro_vba-next"
#	;;
#"gbc")
#	CORES="Libretro_gambatte,Libretro_gearboy,Libretro_tgbdual"
#	;;
#"amiga")
#	CORES="AMIBERRY,Libretro_puae"
#	;;
#"dosbox")
#	CORES="Libretro_dosbox,DOSBOXSDL2"
#	;;
#"dreamcast")
#	CORES="REICASTSA,Libretro_reicast"
#	;;
#"neocd")
#	CORES="Libretro_libneocd,Libretro_fbalpha"
#	;;
#esac
