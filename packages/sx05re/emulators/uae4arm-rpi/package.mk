# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="uae4arm-rpi"
PKG_VERSION="38591d3961e07470b4b615353aa9fc80d0ba2bc9"
PKG_SHA256="06304c5d1c682bcf297e8ac27e2390747b45888732b352aacb7ee1abe17643b3"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="MAME"
PKG_SITE="https://github.com/Chips-fr/uae4arm-rpi"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL SDL_image SDL_ttf SDL_gfx guichan libogg flac"
PKG_LONGDESC="Port of uae4arm on Raspberry Pi. "
PKG_TOOLCHAIN="make"

pre_configure_target() {
CPPFLAGS="$TARGET_CPPFLAGS -I${SYSROOT_PREFIX}/usr/include/SDL"
PKG_MAKE_OPTS_TARGET+=" PLATFORM=gles CC=$CC CXX=$CXX AR=$AR"
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  cp uae4arm $INSTALL/usr/bin/uae4arm
}
