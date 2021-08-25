# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_VERSION="9e33b45573899115015bd715c4d6a5714f3a146d"
PKG_SHA256="0779e791253d2bf4073317cf2c4d4cd42f65a5f2313c23f5319434da96f07732"
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
