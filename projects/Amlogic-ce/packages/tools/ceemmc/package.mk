# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_LICENSE="proprietary"
PKG_SITE="https://coreelec.org"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Tool to install CoreELEC on internal eMMC"
PKG_TOOLCHAIN="manual"
PKG_VERSION="b8b56af20b04e5d2e7b0968a257ec7ce5b0c779a"

case "${ARCH}" in
  arm)
    PKG_SHA256="4cf138ac8b52ebd08ef7c7823e9e90dd37cbf6a69e5699adb2becb315c1dc826"
    ;;
  aarch64)
    PKG_SHA256="907f652754c611cd39caf562508fba1d25c35cc661e2349102510660ccaa13e5"
    ;;
esac

PKG_SOURCE_NAME="${PKG_NAME}-${ARCH}-${PKG_VERSION}.tar.xz"
PKG_URL="https://sources.coreelec.org/${PKG_SOURCE_NAME}"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/sbin
    install -m 0755 ceemmc ${INSTALL}/usr/sbin/ceemmc
}
