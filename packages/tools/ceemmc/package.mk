# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_VERSION="2c1e661b2fad7959c7b2f0dfd05dab2504023de8"
PKG_SHA256="6e3a5331236492054e8b14a921ec546b3b7933bf6d246bf5bc49aa6a4aa3d627"
PKG_SOURCE_NAME="$PKG_NAME-$ARCH-$PKG_VERSION.tar.xz"
PKG_LICENSE="proprietary"
PKG_SITE="https://coreelec.org"
PKG_URL="https://sources.coreelec.org/$PKG_SOURCE_NAME"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Tool to install CoreELEC on internal eMMC"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/usr/sbin
    install -m 0755 ceemmc $INSTALL/usr/sbin/ceemmc
}
