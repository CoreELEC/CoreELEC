# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="beetle-psx-hw"
PKG_VERSION="a1fdfc08b83af029484234ebc8ea7800ac1db7ee"
PKG_SHA256="e438cbacd48371ee44d3a463ed02a130f0d4e250d0ae3aad719ce2a810337c70"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="Non-commercial"
PKG_SITE="https://github.com/ZachCook/beetle-psx-libretro"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="emuelec"
PKG_SHORTDESC="Fork of Mednafen PSX "
PKG_TOOLCHAIN="make"
PKG_GIT_CLONE_BRANCH="lightrec"

pre_configure_target() {
  PKG_MAKE_OPTS_TARGET+=" HAVE_HW=1 HAVE_OPENGL=0 platform=unix-gles"
}

makeinstall_target() {
 mkdir -p $INSTALL/usr/lib/libretro
 cp *_libretro.so $INSTALL/usr/lib/libretro/
}
