# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_VERSION="ec00a72ade7a2fec848cc4e918501b5053135333"
PKG_SHA256="a84f47edbc99cb85479c3187c95cbd124abbbb232ec2c4988794da8086072951"
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
