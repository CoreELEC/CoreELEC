#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Team CoreELEC (https://coreelec.org)

WAIT=1
TIMEOUT=60
SYSTEMDISPCAP="/sys/class/amhdmitx/amhdmitx0/disp_cap"
USERDIR="/storage/.kodi/userdata"
USERDISPCAP="$USERDIR/disp_cap"
USERDISPADD="$USERDIR/disp_add"
TMPDISPADD="/run/disp_add"

START=0
TIMEWAITED=0
if (! grep -q '[^[:space:]]' "$USERDISPCAP" 2> /dev/null); then
  while (! grep -q '[^[:space:]]' "$SYSTEMDISPCAP" 2> /dev/null) && [ $TIMEWAITED -le $TIMEOUT ]; do
    if [ $START -eq 0 ];then
      echo "Display not ready. No disp_cap."
      START=1
    fi
    sleep "$WAIT"
    TIMEWAITED=$(( TIMEWAITED + WAIT ))
  done
  if [ $TIMEWAITED -ge $TIMEOUT ];then
    echo "Timeout: Display still not ready."
    echo "Starting Kodi anyway."
    if [ ! -f "$USERDISPADD" ]; then
      echo 720p60hz > "$TMPDISPADD"
    fi
  fi
fi
