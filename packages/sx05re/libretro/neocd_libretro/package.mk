# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="neocd_libretro"
PKG_VERSION="355fa8e9ddd22eb04edc2f2dd273521c52e0d3b4"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="LGPLv3.0"
PKG_SITE="https://github.com/libretro/neocd_libretro"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain flac libogg libvorbis"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Neo Geo CD emulator for libretro "
PKG_LONGDESC="NeoCD-Libretro is a complete rewrite of NeoCD from scratch in modern C++11. It is designed with accuracy and portability in mind rather than being all about speed like the the older versions. The goal is also to document all I know about the platform in the source code so other emulator authors can make their own implementations."
PKG_IS_ADDON="no"
PKG_TOOLCHAIN="cmake-make"
PKG_AUTORECONF="no"
GET_HANDLER_SUPPORT="git"

pre_configure_target() {
  export CFLAGS="$CFLAGS -Ofast -fomit-frame-pointer -ffast-math -D_7ZIP_ST"
  export CXXFLAGS="$CXXFLAGS -Ofast -fomit-frame-pointer -ffast-math -std=c++11 -fno-exceptions -fno-rtti"
}

make_target() {
  cmake -G "Unix Makefiles" .
  make
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp $PKG_BUILD/.$TARGET_NAME/libneocd_libretro.so $INSTALL/usr/lib/libretro/
}
