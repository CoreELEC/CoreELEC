# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="rs97-commander-sdl2"
PKG_VERSION="4bc1bf2b08e94bcd17d5ed93fb0c3d4193b05d28"
PKG_SHA256="8b09adc1204cb9b18232f38baf98943a7aa02deb932e1c3327a9f6417ef7848d"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/zfteam/rs97-commander-sdl2"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2_image SDL2_gfx"
PKG_PRIORITY="optional"
PKG_SECTION="tools"
PKG_SHORTDESC="Two-pane commander for RetroFW and RG-350 (fork of Dingux Commander)"

pre_configure_target() {
sed -i "s|sdl2-config|${SYSROOT_PREFIX}/usr/bin/sdl2-config|" Makefile
sed -i "s|CC=g++|CC=${CXX}|" Makefile
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  cp DinguxCommander $INSTALL/usr/bin/
  cp -rf res $INSTALL/usr/bin/
}
