#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

. /etc/profile
oe_setup_addon script.program.steamlink

# Steamlink not ready; abort
if [ ! -f ${ADDON_DIR}/prep.ok ]; then
  exit 0
fi

# Adapt to meet steamlink requirements

# mount an overlay for udev rules and reload udev rules
if [ $(grep -c "/lib/udev/rules.d" /proc/mounts) -eq 0 ]; then
  if [ ! -d ${ADDON_DIR}/steamlink/.overlay ]; then
    mkdir -p ${ADDON_DIR}/steamlink/.overlay
  fi
  mount -t overlay overlay -o lowerdir=/lib/udev/rules.d,upperdir=${ADDON_DIR}/steamlink/udev/rules.d/,workdir=${ADDON_DIR}/steamlink/.overlay /lib/udev/rules.d
  udevadm trigger
fi

# Launch steamlink
# controller input goes to kodi if it is running
systemctl stop kodi
${ADDON_DIR}/steamlink/steamlink.sh
# Cleanup
umount /lib/udev/rules.d
udevadm trigger
systemctl start kodi

exit 0
