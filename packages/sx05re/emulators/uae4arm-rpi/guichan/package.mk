# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="guichan"
PKG_VERSION="1a727941539e7ed4376dc8194cb4988961c86340"
PKG_SHA256=""
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="MAME"
PKG_SITE="https://github.com/sphaero/guichan"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL_image"
PKG_LONGDESC="Original Guichan 0.8.2 with some fixes  "
PKG_TOOLCHAIN="make"

make_target() {
CPPFLAGS+="-I${SYSROOT_PREFIX}/usr/include/SDL"

mkdir -p $SYSROOT_PREFIX/usr/include/guichan/
cp -r $PKG_BUILD/include/guichan/* $SYSROOT_PREFIX/usr/include/guichan/

cd $PKG_BUILD
./configure --prefix=/usr --datadir=/usr/share/ --datarootdir=/usr/share/ --host=armv8a-libreelec-linux --disable-opengl --enable-sdl
make
} 

