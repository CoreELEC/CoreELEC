# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mupen64plus-nx"
if [ $PROJECT = "Amlogic" ]; then
PKG_VERSION="81a58df0263c90b10b7fc11b6deee04d47e3aa40"
PKG_SHA256="b8967a8f3383cced4c9df3f297dc5653a56554074af1b5dfff20a2b67475f620"
else
PKG_VERSION="39b555e52bc8821c15f5aa33e366285cd9272630"
PKG_SHA256="3c844fd77095150694f526aad158ad7183f94b848c12a26299bac94202f9fef8"
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
}

if [ ${PROJECT} = "Amlogic-ng" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=AMLG12 GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
elif [ "${PROJECT}" = "Amlogic" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=AMLGX GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
elif [ "${DEVICE}" = "OdroidGoAdvance" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=odroidgoa"
fi

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mupen64plus_next_libretro.so $INSTALL/usr/lib/libretro/
}
