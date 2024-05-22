# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="libamcodec"
PKG_LICENSE="proprietary"
PKG_SITE="http://openlinux.amlogic.com"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libamplayer: Interface library for Amlogic media codecs"
PKG_TOOLCHAIN="manual"

case "${DEVICE}" in
  Amlogic-ng)
    PKG_VERSION="8a779caba41b163fed9ebd0df82db36dd962a9a3"
    PKG_SHA256="72aa29376f560994faa8b76cd4d9f8d6790bd2d88748fcf59ed66115e62d3d3f"
    ;;
  Amlogic-ne)
    PKG_VERSION="20f0e1c9c3c0334d2fa0cf639ee66f66ac63c7da"
    PKG_SHA256="d28745116e0b8bdeb57190b99ed5c71f9173f7507c6ea4c43d982ef094c9cb37"
    ;;
esac

PKG_SOURCE_NAME="${PKG_NAME}-${ARCH}-${PKG_VERSION}.tar.xz"
PKG_URL="https://sources.coreelec.org/${PKG_SOURCE_NAME}"

make_target() {
  cp -PR * $SYSROOT_PREFIX
}

makeinstall_target() {
  mkdir -p $INSTALL/usr
    cp -PR usr/lib $INSTALL/usr
}
