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

PKG_NAME="fuse-libretro"
PKG_VERSION="042f8a9d4758d2d9a47ae064a1fe76b73ad9282c"
PKG_SHA256="16f709c064a7d25b7f82d3f18b95e6372a2ea82dc50a177568a5832303c1938a"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/libretro/fuse-libretro"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="A port of the Fuse Unix Spectrum Emulator to libretro "
PKG_LONGDESC="A port of the Fuse Unix Spectrum Emulator to libretro "

PKG_TOOLCHAIN="make"

make_target() {
if [ "${DEVICE}" == "Amlogic-ng" ]; then
  make -f Makefile.libretro platform=rpi4_64
 else
  make -f Makefile.libretro platform=rpi3_64 
fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp fuse_libretro.so $INSTALL/usr/lib/libretro/
}
