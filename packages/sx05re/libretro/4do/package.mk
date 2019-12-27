################################################################################
#      This file is part of OpenELEC - http://www.openelec.tv
#      Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
#
#  This Program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2, or (at your option)
#  any later version.
#
#  This Program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with OpenELEC.tv; see the file COPYING.  If not, write to
#  the Free Software Foundation, 51 Franklin Street, Suite 500, Boston, MA 02110, USA.
#  http://www.gnu.org/copyleft/gpl.html
################################################################################

PKG_NAME="4do"
PKG_VERSION="15a775bd2da38c6d10a0f76db3f8429d103a6ae8"
PKG_SHA256="86fa3aaa1dbb5a8ecf64354c7aacac0ba434aedd21983c1a5ad6e67a6a4d24de"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="LGPL with additional notes"
PKG_SITE="https://github.com/libretro/4do-libretro"
PKG_URL="https://github.com/libretro/4do-libretro/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Port of 4DO/libfreedo to libretro."
PKG_LONGDESC="Port of 4DO/libfreedo to libretro."
PKG_TOOLCHAIN="make"


make_target() {
  make CC=$CC CXX=$CXX AR=$AR
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp 4do_libretro.so $INSTALL/usr/lib/libretro/
}
