#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2023-present Team CoreELEC (https://coreelec.org)

# get coreelec release information
source /etc/os-release

message() {
  >&2 echo "${@}"
}

load_dovi_ne() {
  # if mounted from tee-loader don't mount/unmount from dovi-loader
  if ! ls /dev/mapper/dynpart-* &>/dev/null; then
    dmsetup create --concise "$(parse-android-dynparts /dev/super)"
    systemctl set-environment dmsetup_remove=yes
  fi

  if [ -b /dev/mapper/dynpart-odm ]; then
    mountpoint -q /android/odm  || mount -o ro /dev/mapper/dynpart-odm /android/odm
    DOVI_KO="/android/odm/lib/modules/dovi.ko"
    if [ -f ${DOVI_KO} ]; then
      modinfo ${DOVI_KO}
      insmod  ${DOVI_KO}
      return
    fi
  fi

  cleanup_dovi_ne
}

cleanup_dovi_ne() {
  rmmod dovi 2>/dev/null
  mountpoint -q /android/odm && umount /android/odm
  # unmount only if mounted from this script
  [ "${dmsetup_remove}" = "yes" ] && \
    ls /dev/mapper/dynpart-* &>/dev/null && dmsetup remove /dev/mapper/dynpart-*
}

load_dovi_ng() {
  mountpoint -q /android/vendor || mount -o ro /dev/vendor /android/vendor
  DOVI_KO="/android/vendor/lib/modules/dovi.ko"
  if [ -f ${DOVI_KO} ]; then
    message "loading dovi module"
    modinfo ${DOVI_KO}
    insmod  ${DOVI_KO}
    return
  fi

  cleanup_dovi_ng
}

cleanup_dovi_ng() {
  rmmod dovi 2>/dev/null
  mountpoint -q /android/vendor && umount /android/vendor
}

message "run ${1} for ${COREELEC_DEVICE:8:2}"

case "${1}" in
  start)
    load_dovi_${COREELEC_DEVICE:8:2}
    ;;
  stop)
    cleanup_dovi_${COREELEC_DEVICE:8:2}
    ;;
esac

exit 0
