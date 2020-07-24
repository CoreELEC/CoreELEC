# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mupen64plus-nx"
PKG_VERSION="be9b85ad17c7bcad5de353ad22e81c7bf6d77c48"
PKG_SHA256="8f3b82ad6fc06885f0fef084fbaf9837aed95bb7efe5e05af29e7e25537b004e"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/mupen64plus-libretro-nx"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain nasm:host $OPENGLES"
PKG_SECTION="libretro"
PKG_SHORTDESC="mupen64plus + RSP-HLE + GLideN64 + libretro"
PKG_LONGDESC="mupen64plus + RSP-HLE + GLideN64 + libretro"
PKG_TOOLCHAIN="make"
PKG_BUILD_FLAGS="-lto"

if [ $PROJECT = "Amlogic" ]; then
PKG_VERSION="b785150465048fa88f812e23462f318e66af0be0"
PKG_SHA256="456c433f45b0e2ba15a587978234e3e1300301d431b6823747ad0e779331c97e"
fi

pre_configure_target() {
  sed -e "s|^GIT_VERSION ?.*$|GIT_VERSION := \" ${PKG_VERSION:0:7}\"|" -i Makefile

if [ $ARCH == "arm" ]; then
if [ ${PROJECT} = "Amlogic-ng" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=AMLG12B GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
	sed -i "s|GLES3 = 1|GLES = 1|g" Makefile
elif [ "${PROJECT}" = "Amlogic" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=AMLGX GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
elif [ "${DEVICE}" = "OdroidGoAdvance" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=odroidgoa"
else
	if [ ${PROJECT} = "Amlogic-ng" ]; then
		PKG_MAKE_OPTS_TARGET+=" platform=odroid64 BOARD=N2"
	elif [ "${PROJECT}" = "Amlogic" ]; then
		PKG_MAKE_OPTS_TARGET+=" platform=amlogic64"
	fi
fi
}


makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mupen64plus_next_libretro.so $INSTALL/usr/lib/libretro/
}