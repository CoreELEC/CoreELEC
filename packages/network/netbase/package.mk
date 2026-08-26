# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="netbase"
PKG_VERSION="6.6"
PKG_SHA256="31f7e8a37f3f07010e484f74bcbee503923b38d1a36c6a11326b804acc07224e"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://salsa.debian.org/md/netbase"
PKG_URL="http://ftp.debian.org/debian/pool/main/n/netbase/netbase_${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="The netbase package provides data for network services and protocols from the iana db."
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p ${INSTALL}/etc
    cp etc/protocols ${INSTALL}/etc
    cp etc/services ${INSTALL}/etc
}
