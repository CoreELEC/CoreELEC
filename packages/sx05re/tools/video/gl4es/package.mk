# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="gl4es"
PKG_VERSION="ebb7dd232fc50cd1698a602723166fcb96443116"
PKG_SHA256="5599ad73505d6eb425c66c77abd97fccc1f05e75553d82926074a171e25b5f1a"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/ptitSeb/gl4es"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="gl4es"
PKG_TOOLCHAIN="cmake-make"

make_target() {
cmake . -DODROID=1 
make GL
}

makeinstall_target() {
PKG_LIBNAME="libGL.so"
PKG_LIBPATH="$PKG_BUILD/.${TARGET_NAME}/lib/libGL.so.1"
PKG_LIBVAR="GL"

  mkdir -p $INSTALL/usr/lib
  cp $PKG_LIBPATH $SYSROOT_PREFIX/usr/lib/$PKG_LIBNAME
  cp $PKG_LIBPATH $INSTALL/usr/lib
  
}
