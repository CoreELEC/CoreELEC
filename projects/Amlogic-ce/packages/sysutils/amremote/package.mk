# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="amremote"
PKG_VERSION="c4867968d3df3bf02387fa681c9e4ab47b315a8f"
PKG_SHA256=""
PKG_LICENSE="other"
PKG_SITE="http://www.amlogic.com"
PKG_URL="https://github.com/CoreELEC/amremote/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain usbutils"
PKG_LONGDESC="amremote - IR remote configuration utility for Amlogic-based devices"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp remotecfg ${INSTALL}/usr/bin

  mkdir -p ${INSTALL}/usr/lib/coreelec
    cp ${PKG_DIR}/scripts/* ${INSTALL}/usr/lib/coreelec
}

post_install() {
  enable_service remote-config.service
  enable_service remote-config-tx.service
}
