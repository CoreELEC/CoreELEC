# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mactool"
PKG_VERSION="1.0"
PKG_LICENSE="GPL-2.0-only"
PKG_DEPENDS_TARGET="toolchain bluez"
PKG_LONGDESC="mactool: create persistent per-device ethernet and Bluetooth addresses from the CPU serial"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp ${PKG_DIR}/scripts/mactool-config ${INSTALL}/usr/bin
}

post_install() {
  enable_service mactool-eth.service
}
