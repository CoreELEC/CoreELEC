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

PKG_NAME="craft"
PKG_VERSION="d348b662032ad615be51dac4f86e6ac23c21b3c0"
PKG_SHA256="537175214b649a2a98423329ab758850731e02c4c0ff0a68ab31e16ab92432e6"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/libretro/Craft"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="A simple Minecraft clone written in C using modern OpenGL (shaders)."
PKG_LONGDESC="A simple Minecraft clone written in C using modern OpenGL (shaders)."

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"

pre_configure_target() {
  cd $PKG_BUILD
}

make_target() {
  if [ "$OPENGLES_SUPPORT" = yes ]; then
    make -f Makefile.libretro FORCE_GLES=1
  else
    make -f Makefile.libretro
  fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp craft_libretro.so $INSTALL/usr/lib/libretro/
}
