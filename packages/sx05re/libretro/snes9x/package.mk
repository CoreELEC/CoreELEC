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

PKG_NAME="snes9x"
PKG_VERSION="a20e726ca274ba454756dd7262ad7bf565c7b04c"
PKG_SHA256="814ef782bac5ca8a25bd316e9721319d5658825940a2568e7f46bb8e0c7a46c2"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="Non-commercial"
PKG_SITE="https://github.com/snes9xgit/snes9x"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Snes9x - Portable Super Nintendo Entertainment System (TM) emulator"
PKG_LONGDESC="Snes9x - Portable Super Nintendo Entertainment System (TM) emulator"

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"

make_target() {
  if [ "$ARCH" == "arm" ]; then
    CXXFLAGS="$CXXFLAGS -DARM"
  fi
  
  if [ "$DEVICE" == "OdroidGoAdvance" ];then 
    make -C libretro platform=classic_armv8_a35
   else
   make -C libretro
   fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp libretro/snes9x_libretro.so $INSTALL/usr/lib/libretro/
}
