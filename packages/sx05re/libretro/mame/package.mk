# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019 Trond Haugland (trondah@gmail.com)

PKG_NAME="mame"
PKG_VERSION="739058dac4d2d2a4553b8677cc54ebe474fea6c3"
PKG_SHA256="b222cf8be9edd97f144edd48bd7328bd1f45d4e93055c7aa8da1d22cb9864bc1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/mame"
PKG_URL="https://github.com/libretro/mame/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain zlib flac sqlite expat"
PKG_SECTION="libretro"
PKG_SHORTDESC="MAME - Multiple Arcade Machine Emulator"
PKG_TOOLCHAIN="make"
PKG_BUILD_FLAGS="-lto"

PTR64="0"
NOASM="0"

if [ "$ARCH" == "arm" ]; then
  NOASM="1"
elif [ "$ARCH" == "x86_64" ]; then
  PTR64="1"
fi

PKG_MAKE_OPTS_TARGET="REGENIE=1 \
		      VERBOSE=1 \
		      NOWERROR=1 \
		      OPENMP=1 \
		      CROSS_BUILD=1 \
		      TOOLS=1 \
		      RETRO=1 \
		      PTR64=$PTR64 \
		      NOASM=$NOASM \
		      PYTHON_EXECUTABLE=python3 \
		      CONFIG=libretro \
		      LIBRETRO_OS=unix \
		      LIBRETRO_CPU=$ARCH \
		      PLATFORM=$ARCH \
		      ARCH= \
		      TARGET=mame \
		      SUBTARGET=arcade \
		      OSD=retro \
		      USE_SYSTEM_LIB_EXPAT=1 \
		      USE_SYSTEM_LIB_ZLIB=1 \
		      USE_SYSTEM_LIB_FLAC=1 \
		      USE_SYSTEM_LIB_SQLITE3=1"

make_target() {
  unset ARCH
  unset DISTRO
  unset PROJECT
  make $PKG_MAKE_OPTS_TARGET OVERRIDE_CC=$CC OVERRIDE_CXX=$CXX OVERRIDE_LD=$LD AR=$AR $MAKEFLAGS
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp *.so $INSTALL/usr/lib/libretro/mame_libretro.so
  mkdir -p $INSTALL/usr/config/retroarch/savefiles/mame/hi
  cp plugins/hiscore/hiscore.dat $INSTALL/usr/config/retroarch/savefiles/mame/hi
}
