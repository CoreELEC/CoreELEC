#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

set -e

. /etc/profile
oe_setup_addon script.program.steamlink

UDEV_DIR="/lib/udev/rules.d"
UPPER="${ADDON_DIR}/steamlink/udev/rules.d"
WORK="${ADDON_DIR}/steamlink/.overlay"

# Steamlink not ready; abort
if [ ! -f "${ADDON_DIR}/prep.ok" ]; then
  exit 0
fi

# Cleanup routine on exit
cleanup() {
  # Unmount udev overlay
  if mountpoint -q "${UDEV_DIR}"; then
    umount "${UDEV_DIR}"
    udevadm control --reload-rules
    udevadm trigger
  fi

  # Start Kodi if not running
  if ! systemctl is-active --quiet kodi; then
    systemctl start kodi
  fi
}
trap cleanup EXIT INT TERM

# Create overlay directory
if [ ! -d "${WORK}" ]; then
  mkdir -p "${WORK}" || {
    echo "Error: Failed to create directory: ${WORK}" >&2
    exit 1
  }
fi

# Mount udev rules overlay and reload rules
if ! mountpoint -q "${UDEV_DIR}"; then
  mount -t overlay overlay \
    -o "lowerdir=${UDEV_DIR},upperdir=${UPPER},workdir=${WORK}" \
    "${UDEV_DIR}"
  udevadm control --reload-rules
  udevadm trigger
fi

# Stop Kodi so controller input only goes to Steam Link
systemctl stop kodi || echo "Warning: Kodi did not stop cleanly"

# Launch Steam Link
"${ADDON_DIR}/steamlink/steamlink.sh"
STATUS=$?

exit $STATUS
