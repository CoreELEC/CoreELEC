# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="rs97-commander-sdl2"
PKG_VERSION="30a9f6a588d220ac233b4c02c8c55fb1e24aeeeb"
PKG_SHA256="441aa06ca35ac0e56cfbfa45522832f50794f2f95475305ce3dddc6100e6693d"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/EmuELEC/rs97-commander-sdl2"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2_image SDL2_gfx"
PKG_PRIORITY="optional"
PKG_SECTION="tools"
PKG_SHORTDESC="Two-pane commander for RetroFW and RG-350 (fork of Dingux Commander)"

pre_configure_target() {
sed -i "s|sdl2-config|${SYSROOT_PREFIX}/usr/bin/sdl2-config|" Makefile
sed -i "s|CC=g++|CC=${CXX}|" Makefile

[[ "$DEVICE" == "OdroidGoAdvance" ]] &&	OGA=1 || OGA=0

PKG_MAKE_OPTS_TARGET=" ODROIDGO=${OGA} CC=$CXX"
	
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  cp DinguxCommander $INSTALL/usr/bin/
  cp -rf res $INSTALL/usr/bin/
}
