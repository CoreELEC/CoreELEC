#!/bin/sh

################################################################################
#      This file is part of LibreELEC - https://libreelec.tv
#      Copyright (C) 2017-present Team LibreELEC
#
#  LibreELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  LibreELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with LibreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

HDMI_UNPLUGGED=0
while :; do
  ! grep -q . /sys/class/amhdmitx/amhdmitx0/disp_cap &&
    grep -q 0 /sys/class/amhdmitx/amhdmitx0/hpd_state &&
    HDMI_UNPLUGGED=1 && sleep 2 && continue ||
  break
done
[ $HDMI_UNPLUGGED = 1 ] && systemctl restart kodi
