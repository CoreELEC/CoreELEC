# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="beetle-psx-hw"
PKG_VERSION="0755bc87cfd4bc3df7934e4a2aed93f4fbb28b09"
PKG_SHA256=""
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/ZachCook/beetle-psx-libretro"
PKG_URL="PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SHORTDESC="Fork of Mednafen PSX"
PKG_TOOLCHAIN="make"
PKG_GIT_CLONE_BRANCH="lightrec"

make_target() {
  make HAVE_HW=1
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mednafen_lynx_libretro.so $INSTALL/usr/lib/libretro/
}
