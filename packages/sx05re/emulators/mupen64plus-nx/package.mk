# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mupen64plus-nx"
if [ $PROJECT = "Amlogic" ]; then
PKG_VERSION="b785150465048fa88f812e23462f318e66af0be0"
PKG_SHA256="456c433f45b0e2ba15a587978234e3e1300301d431b6823747ad0e779331c97e"
else
PKG_VERSION="f68044fe275ef49ea142a304af873aa08aca78a3"
PKG_SHA256="2e3a1626cec472223ad5832f1447303fd1e0855ab6be5163fe082a906bac1a26"
fi
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

if [ ${PROJECT} = "Amlogic-ng" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=AMLG12B GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
	sed -i "s|GLES3 = 1|GLES = 1|g" Makefile
elif [ "${PROJECT}" = "Amlogic" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=AMLGX GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
elif [ "${DEVICE}" = "OdroidGoAdvance" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=odroidgoa"
fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mupen64plus_next_libretro.so $INSTALL/usr/lib/libretro/
}
