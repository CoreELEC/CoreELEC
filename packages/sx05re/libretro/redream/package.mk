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

PKG_NAME="redream"
PKG_VERSION="c656735ffaa29ccca3fdb0384ceb3bddb9aa6c7a"
PKG_SHA256="938d742507f548c1156fb3b4ecdfdf1c4ffb351eb5532052f8e19446dd181287"
PKG_REV="1"
PKG_ARCH="arm i386 x86_64"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/libretro/retrodream"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_LONGDESC="Work In Progress SEGA Dreamcast emulator"
PKG_TOOLCHAIN="make"
PKG_USE_CMAKE="no"

make_target() {
  cd $PKG_BUILD/deps/libretro
  if [ "$ARCH" == "arm" ]; then
    make CC=$CC CXX=$CXX FORCE_GLES=1 WITH_DYNAREC=arm HAVE_NEON=1
  else
    make
  fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp $PKG_BUILD/deps/libretro/retrodream_libretro.so $INSTALL/usr/lib/libretro/
}
