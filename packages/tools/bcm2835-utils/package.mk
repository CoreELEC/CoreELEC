# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bcm2835-utils"
PKG_VERSION="60ea0df9b7a43b4198c0def6355b600eb2cfbfb1"
PKG_SHA256="3fe48591672f35e7e9a756437f719530ab861b8e4f380594783281bdd30401dd"
PKG_ARCH="arm aarch64"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://github.com/raspberrypi/utils"
PKG_URL="https://github.com/raspberrypi/utils/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="cmake:host gcc:host dtc"
PKG_LONGDESC="Raspberry Pi related collection of scripts and simple applications"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/vclog/vclog ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/dtmerge/{dtoverlay,dtmerge,dtparam} ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/pinctrl/pinctrl ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/vcgencmd/vcgencmd ${INSTALL}/usr/bin
  cp -PRv ${PKG_BUILD}/.${TARGET_NAME}/vcmailbox/vcmailbox ${INSTALL}/usr/bin
}
