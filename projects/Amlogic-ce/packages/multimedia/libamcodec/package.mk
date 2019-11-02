# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="libamcodec"
PKG_VERSION="5fa8d401f3d00e7cc0e0b34bf19a0d6a51561603"
PKG_SHA256="1dfede5dee6c112170327c31d164c776e464aac3abfb3bc4e0cafd0c75ab12ef"
PKG_LICENSE="proprietary"
PKG_SITE="http://openlinux.amlogic.com"
PKG_URL="https://github.com/CoreELEC/libamcodec/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libamplayer: Interface library for Amlogic media codecs"
PKG_TOOLCHAIN="manual"

make_target() {
  cp -PR * $SYSROOT_PREFIX
}

makeinstall_target() {
  cp -PR * $INSTALL
}
