# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="gl4es"
PKG_VERSION="b3592d90798cea9f3d40c1c384bde3b281e57cf0"
PKG_SHA256="e7281d7d5b1015943b672b720a3524f1db5526b9f3b26cdab2eed619d0b3c12f"
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
PKG_LIBPATH="$PKG_BUILD/.armv8a-libreelec-linux-gnueabi/lib/libGL.so.1"
PKG_LIBVAR="GL"

  mkdir -p $INSTALL/usr/lib
  cp $PKG_LIBPATH $SYSROOT_PREFIX/usr/lib/$PKG_LIBNAME
  cp $PKG_LIBPATH $INSTALL/usr/lib
  
}
