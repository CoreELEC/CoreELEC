# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="libamcodec"
PKG_VERSION="fbbe9fa6fdb223824ef8ae627e02ee7aee1f4abf"
PKG_SHA256="eff3ead4e7ee0a1f032b9c1db51cc2b4094a61beb9ea6560da4663743292b36c"
PKG_LICENSE="proprietary"
PKG_SITE="http://openlinux.amlogic.com"
PKG_URL="https://sources.coreelec.org/$PKG_NAME-$ARCH-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libamplayer: Interface library for Amlogic media codecs"
PKG_TOOLCHAIN="manual"

make_target() {
  cp -PR * $SYSROOT_PREFIX
}

makeinstall_target() {
  mkdir -p $INSTALL/usr
    cp -PR usr/lib $INSTALL/usr
}
