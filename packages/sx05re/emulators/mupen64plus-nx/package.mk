# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mupen64plus-nx"
PKG_VERSION="f77c16f9f1dd911fd2254becc8a28adcdafe8aa1"
PKG_SHA256="a6972d0a423778a8447cbe69c2c7d4179fdd1bbef3a3d37ea823ba554ff8f4d8"
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


pre_configure_target() {
  sed -e "s|^GIT_VERSION ?.*$|GIT_VERSION := \" ${PKG_VERSION:0:7}\"|" -i Makefile
}

if [ ${PROJECT} = "Amlogic-ng" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=AMLG12 GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
elif [ "${PROJECT}" = "Amlogic" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=AMLGX GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
fi

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mupen64plus_next_libretro.so $INSTALL/usr/lib/libretro/
}
