# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mupen64plus-nx"
PKG_VERSION="b785150465048fa88f812e23462f318e66af0be0"
PKG_SHA256="456c433f45b0e2ba15a587978234e3e1300301d431b6823747ad0e779331c97e"
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
PKG_EE_UPDATE="no"

pre_configure_target() {
  sed -e "s|^GIT_VERSION ?.*$|GIT_VERSION := \" ${PKG_VERSION:0:7}\"|" -i Makefile

if [ $ARCH == "arm" ]; then
	if [ ${PROJECT} = "Amlogic-ng" ]; then
		PKG_MAKE_OPTS_TARGET+=" platform=AMLG12B"
	elif [ "${PROJECT}" = "Amlogic" ]; then
		PKG_MAKE_OPTS_TARGET+=" platform=amlogic"
	elif [ "${DEVICE}" = "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
		sed -i "s|cortex-a53|cortex-a35|g" Makefile
		PKG_MAKE_OPTS_TARGET+=" platform=odroidgoa"
	fi
else
	if [ ${PROJECT} = "Amlogic-ng" ]; then
		PKG_MAKE_OPTS_TARGET+=" platform=odroid64 BOARD=N2"
	elif [ "${PROJECT}" = "Amlogic" ]; then 
		PKG_MAKE_OPTS_TARGET+=" platform=amlogic64"
	elif [ "${DEVICE}" = "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
		PKG_MAKE_OPTS_TARGET+=" platform=amlogic64"
	fi
fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mupen64plus_next_libretro.so $INSTALL/usr/lib/libretro/
}
