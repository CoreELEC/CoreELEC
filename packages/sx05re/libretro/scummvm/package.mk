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

PKG_NAME="scummvm"
PKG_VERSION="63e57573a9ffe71a1083ff46d9cd210203b87afb"
PKG_SHA256="b5be836e068caf16418d3fde87c6ba436b31d87e1d9627855ba463bb0841a5d5"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/scummvm"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="ScummVM with libretro backend."
PKG_LONGDESC="ScummVM is a program which allows you to run certain classic graphical point-and-click adventure games, provided you already have their data files."

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"
PKG_BUILD_FLAGS="-lto"

pre_configure_target() {
  cd ..
  rm -rf .$TARGET_NAME
}

configure_target() {
  :
}

make_target() {
if ([ "$DEVICE" == "OdroidGoAdvance" ] || [ "$DEVICE" == "OdroidGoAdvance" ]) && [ "$ARCH" == "arm" ]; then
 make -C backends/platform/libretro/build platform=oga_a35_neon_hardfloat CXXFLAGS="`echo $CXXFLAGS | sed -e "s|-O.|-O2|g"` -DHAVE_POSIX_MEMALIGN=1"
else
 make -C backends/platform/libretro/build CXXFLAGS="$CXXFLAGS -DHAVE_POSIX_MEMALIGN=1"
fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp backends/platform/libretro/build/scummvm_libretro.so $INSTALL/usr/lib/libretro/
}
