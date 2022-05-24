# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_LICENSE="proprietary"
PKG_SITE="https://coreelec.org"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Tool to install CoreELEC on internal eMMC"
PKG_TOOLCHAIN="manual"

case "${DEVICE}" in
  Amlogic-ng)
    PKG_VERSION="e7be656a035a85da07fdfae4761e031120826943"
    PKG_SHA256="6c923ac1c022228533b0d19ebb58fc665c3a53a1924c5b5269d6b8795c025c57"
    ;;
  Amlogic-ne)
    PKG_VERSION="6844e09f62e6069f35e344cc6dba437d2062bb32"
    PKG_SHA256="4e86af206ed3389caa71ca4fa2a35e5c97aa19a7cb83401365cfabd0e0e33979"
    ;;
esac

PKG_SOURCE_NAME="${PKG_NAME}-${ARCH}-${PKG_VERSION}.tar.xz"
PKG_URL="https://sources.coreelec.org/${PKG_SOURCE_NAME}"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/sbin
    install -m 0755 ceemmc ${INSTALL}/usr/sbin/ceemmc
}
