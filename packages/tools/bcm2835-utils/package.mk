# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bcm2835-utils"
PKG_VERSION="a30e7c7b227d9a5e6dbedc1d343077be7ad92959"
PKG_SHA256="0ccde5f9d0bc019b75b097769bd68f950d86950ff1d66280d4c163ee4a6f424a"
PKG_ARCH="arm aarch64"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://github.com/raspberrypi/utils"
PKG_URL="https://github.com/raspberrypi/utils/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="cmake:host gcc:host dtc ncurses"
PKG_LONGDESC="Raspberry Pi related collection of scripts and simple applications"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/vclog/vclog ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/dtmerge/{dtoverlay,dtmerge,dtparam} ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/pinctrl/pinctrl ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/vcgencmd/vcgencmd ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/vcmailbox/vcmailbox ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/rpi-gpu-usage/rpi-gpu-usage ${INSTALL}/usr/bin
}
