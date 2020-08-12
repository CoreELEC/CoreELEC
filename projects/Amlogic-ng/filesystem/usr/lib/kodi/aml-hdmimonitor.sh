#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

HDMI_UNPLUGGED=0
while :; do
  ! grep -q . /sys/class/amhdmitx/amhdmitx0/disp_cap &&
    grep -q 0 /sys/class/amhdmitx/amhdmitx0/hpd_state &&
    HDMI_UNPLUGGED=1 && sleep 2 && continue ||
  break
done

if [ $HDMI_UNPLUGGED = 1 ]; then
  if [ -f /flash/resolution.ini ]; then
    echo null > /sys/class/display/mode
    cat /flash/resolution.ini |grep frac_rate_policy| awk -F "=" '{print $2}' >/sys/devices/virtual/amhdmitx/amhdmitx0/frac_rate_policy
    cat /flash/resolution.ini |grep hdmimode| awk -F "=" '{print $2}' >/sys/class/display/mode
  else
    echo null > /sys/class/display/mode
    echo 1080p60hz >/sys/class/display/mode
  fi
  systemctl restart kodi
fi
