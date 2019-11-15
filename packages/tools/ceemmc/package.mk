# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_VERSION="574211d9ac1384de314e4d3daf5e1d6951971f32"
PKG_SHA256="ff1fe96be37f1128171c6acfbbf0cc642b23b5c478d221e885c5869e665435ee"
PKG_LICENSE="proprietary"
PKG_SITE="https://coreelec.org"
PKG_URL="https://sources.coreelec.org/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Tool to install CoreELEC on internal eMMC"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/usr/sbin
    install -m 0755 ceemmc $INSTALL/usr/sbin/ceemmc
}
