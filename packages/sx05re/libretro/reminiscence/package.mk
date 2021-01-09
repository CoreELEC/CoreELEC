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

PKG_NAME="reminiscence"
PKG_VERSION="500a792483cc7be46da05a9e199081f00c9aa0ed"
PKG_SHA256="1c78591ed5003d2f705e03ffc5e4c0c67b8c0cd313256cbfc09d6e6b85e64e9b"
PKG_ARCH="any"
PKG_SITE="https://github.com/libretro/REminiscence"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Port of Gregory Montoir's Flashback emulator, running as a libretro core."
PKG_LONGDESC="Port of Gregory Montoir's Flashback emulator, running as a libretro core."

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"

configure_target () {
  : # nothing to do
}

make_target() {
  cd $PKG_BUILD
  make
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp reminiscence_libretro.so $INSTALL/usr/lib/libretro/
}
