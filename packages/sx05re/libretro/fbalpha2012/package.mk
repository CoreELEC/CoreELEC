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

PKG_NAME="fbalpha2012"
PKG_VERSION="a292dacd7249b3d8c1178d5c52c54d60f8099323"
PKG_SHA256="4e4f5d37daea26401bc917593d2aa540a544b25fd41c196e064791aad19dfdf0"
PKG_ARCH="any"
PKG_LICENSE="Non-commercial"
PKG_SITE="https://github.com/libretro/fbalpha2012"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Port of Final Burn Alpha 2012 to Libretro"
PKG_TOOLCHAIN="make"

make_target() {
  cd svn-current/trunk
  if [ "$ARCH" == "arm" ]; then
    make -f makefile.libretro platform=armv CC=$CC CXX=$CXX
  else
    make -f makefile.libretro CC=$CC CXX=$CXX
  fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp fbalpha2012_libretro.so $INSTALL/usr/lib/libretro/
}
