#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

. /etc/profile

# Set some common variables
M64P_TMP_DIR=/tmp/emulation/m64p
M64P_LOG=/var/log/m64p.log
M64P_GAME_NAME=$(basename -- "$1")
M64P_VIDEO="/usr/lib/mupen64plus/mupen64plus-video-gliden64.so"
M64P_PARAMS="--emumode 2 --resolution 1280x720  --fullscreen"

# Clean up log file
if [ -f "${M64P_LOG}" ]; then
  rm "${M64P_LOG}"
fi

if [ `echo ${M64P_GAME_NAME} | grep -i  '.n64\|.v64\|.z64\|.bin\|.u1\|.ndd\|.7z\|.zip' | wc -l` -eq 1 ]; then

  # Which file should M64P load?
  echo "Trying to boot this game:" "${M64P_GAME_NAME}" > ${M64P_LOG}

  # Create a clean working directory
  if [ -d "${M64P_TMP_DIR}" ]; then
    echo "Clean up old working directory." >> ${M64P_LOG}
    rm -rf "${M64P_TMP_DIR}"
  fi

  # Check if we are loading a zip file
  if [ `echo ${M64P_GAME_NAME} | grep -i .zip | wc -l` -eq 1 ]; then
    # .zip file detected
    echo "Loading a .zip file..." >> ${M64P_LOG}

    # unpack the zip file
    mkdir -p "${M64P_TMP_DIR}"
    unzip -qq -o "$1" *.n64 *.v64 *.z64 *.bin *.u1 *.ndd -d "${M64P_TMP_DIR}" 2>/dev/null

    # start m64p
    SDL_AUDIODRIVER=alsa mupen64plus ${M64P_PARAMS} --gfx ${M64P_VIDEO} "${M64P_TMP_DIR}"/* >> ${M64P_LOG} 2>&1
    rm -rf "${M64P_TMP_DIR}"

  # Check if we are loading a 7z file
  elif [ `echo ${M64P_GAME_NAME} | grep -i .7z | wc -l` -eq 1 ]; then
    # .7z file detected
    echo "Loading a .7z file..." >> ${M64P_LOG}

	# unpack the 7z file
    mkdir -p "${M64P_TMP_DIR}"
	7za x "$1" -o"${M64P_TMP_DIR}" *.n64 *.v64 *.z64 *.bin *.u1 *.ndd -r > /dev/null

    # start m64p
    SDL_AUDIODRIVER=alsa mupen64plus ${M64P_PARAMS} --gfx ${M64P_VIDEO} "${M64P_TMP_DIR}"/* >> ${M64P_LOG} 2>&1
    rm -rf "${M64P_TMP_DIR}"
  else
    # non-compressed file detcted
    echo "Loading a regular file..." >> ${M64P_LOG}

    SDL_AUDIODRIVER=alsa mupen64plus ${M64P_PARAMS} --gfx ${M64P_VIDEO} "$@" >> ${M64P_LOG} 2>&1  
  fi

else
   echo "File extension of" "${M64P_GAME_NAME}" "is not supported" >> ${M64P_LOG}
   echo "Try a rom with with one of these file extensions .n64 .v64 .z64 .bin .u1 .ndd .7z .zip" >> ${M64P_LOG}
fi
