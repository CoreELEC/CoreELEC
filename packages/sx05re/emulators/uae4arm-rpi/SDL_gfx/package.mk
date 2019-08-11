# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="SDL_gfx"
PKG_VERSION="2.0.26"
PKG_SHA256=""
PKG_LICENSE="GPL"
PKG_SITE="http://www.ferzkopp.net/"
PKG_URL="http://www.ferzkopp.net/Software/SDL_gfx-2.0/SDL_gfx-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL"
PKG_LONGDESC="SDL graphics drawing primitives and other support functions"

configure_target() { 
CPPFLAGS="$TARGET_CPPFLAGS -I${SYSROOT_PREFIX}/usr/include"
cd $PKG_BUILD
./configure --prefix=/usr --datadir=/usr/share/ --datarootdir=/usr/share/ --host=arm --disable-mmx
}

make_target() {
CC=$CC CXX=$CXX AR=$AR make all
}
