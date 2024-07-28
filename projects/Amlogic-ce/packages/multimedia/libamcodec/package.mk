# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="libamcodec"
PKG_VERSION="20f0e1c9c3c0334d2fa0cf639ee66f66ac63c7da"
PKG_SHA256="d28745116e0b8bdeb57190b99ed5c71f9173f7507c6ea4c43d982ef094c9cb37"
PKG_LICENSE="proprietary"
PKG_SITE="http://openlinux.amlogic.com"
PKG_SOURCE_NAME="libamcodec-aarch64-${PKG_VERSION}.tar.xz"
PKG_URL="https://sources.coreelec.org/${PKG_SOURCE_NAME}"
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
