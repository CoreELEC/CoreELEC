# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="hwdata"
PKG_VERSION="0.387"
PKG_SHA256="8c6be8f0863a8ff5c83b2c46aa525b503b30d42792ed57891c40849de543e1ee"
PKG_LICENSE="GPL-2.0"
PKG_SITE="https://github.com/vcrhonek/hwdata"
PKG_URL="https://github.com/vcrhonek/hwdata/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="hwdata contains various hardware identification and configuration data, such as the pci.ids and usb.ids databases"

pre_configure_target() {
  # hwdata fails to build in subdirs
  cd ${PKG_BUILD}
    rm -rf .${TARGET_NAME}

    sed -i "s&@prefix@|&@prefix@|${PKG_INSTALL}&" Makefile
    sed -i "s&prefix=@prefix@&prefix=/usr&" hwdata.pc.in
}
