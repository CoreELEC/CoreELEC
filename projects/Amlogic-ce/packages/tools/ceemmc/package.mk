# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_VERSION="5c89919264c75edade85cd3ad0f0756f42f2ad4a"
PKG_SHA256="bc4ddba5a5fd17cacccbe4200fa8f4b3d89084b01efeec3caad56fc8c8a7e9c8"
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
