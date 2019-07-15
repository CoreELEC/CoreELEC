# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mupen64plus-nx"
PKG_VERSION="b31459d1e9b1bc1cf93d3577e185fe8d4198a978"
PKG_SHA256="0b8a2e62d1349e04625db67c308e72c8cc33156d6379c33e367441af4154479c"
PKG_REV="1"
PKG_ARCH="arm"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/mupen64plus-libretro-nx"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain nasm:host $OPENGLES"
PKG_SECTION="libretro"
PKG_SHORTDESC="mupen64plus + RSP-HLE + GLideN64 + libretro"
PKG_LONGDESC="mupen64plus + RSP-HLE + GLideN64 + libretro"
PKG_TOOLCHAIN="make"
PKG_BUILD_FLAGS="-lto"

PKG_MAKE_OPTS_TARGET+=" platform=amlogic GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"

pre_make_target() { 
if [ ${PROJECT} = "Amlogic-ng" ]; then
	sed -i "s|-mcpu=cortex-a53 -mtune=cortex-a53|-mcpu=cortex-a73 -mtune=cortex-a73.cortex-a53|g" $PKG_BUILD/Makefile
fi 
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mupen64plus_next_libretro.so $INSTALL/usr/lib/libretro/
}
