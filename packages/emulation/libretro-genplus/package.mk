################################################################################
#      This file is part of LibreELEC - https://libreelec.tv
#      Copyright (C) 2016-present Team LibreELEC
#
#  LibreELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  LibreELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with LibreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

PKG_NAME="libretro-genplus"
PKG_VERSION="6ba9e05"
PKG_SHA256="51efda2061d483824bc71c91537dfbfc3edb006cda339bc4c50fc4fc7f93d599"
PKG_ARCH="any"
PKG_LICENSE="Modified BSD / LGPLv2.1"
PKG_SITE="https://github.com/libretro/Genesis-Plus-GX"
PKG_URL="https://github.com/libretro/Genesis-Plus-GX/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="Genesis-Plus-GX-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain kodi-platform"
PKG_SECTION="emulation"
PKG_SHORTDESC="game.libretro.genplus: Genesis Plus GX for Kodi"
PKG_LONGDESC="game.libretro.genplus: Genesis Plus GX for Kodi"
PKG_TOOLCHAIN="manual"

PKG_LIBNAME="genesis_plus_gx_libretro.so"
PKG_LIBPATH="$PKG_LIBNAME"
PKG_LIBVAR="GENPLUS_LIB"

make_target() {
  make -f Makefile.libretro
}

makeinstall_target() {
  mkdir -p $SYSROOT_PREFIX/usr/lib/cmake/$PKG_NAME
  cp $PKG_LIBPATH $SYSROOT_PREFIX/usr/lib/$PKG_LIBNAME
  echo "set($PKG_LIBVAR $SYSROOT_PREFIX/usr/lib/$PKG_LIBNAME)" > $SYSROOT_PREFIX/usr/lib/cmake/$PKG_NAME/$PKG_NAME-config.cmake
}
