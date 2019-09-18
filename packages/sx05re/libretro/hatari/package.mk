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

PKG_NAME="hatari"
PKG_VERSION="ecfc143efce6e068a4b506dfdaf6516d3c268372"
PKG_SHA256="f021698321e7c4cef7634cd33be7e924ddc1165bba6dd2ab534469bace74dcd3"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/hatari"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="New rebasing of Hatari based on Mercurial upstream. Tries to be a shallow fork for easy upstreaming later on."
PKG_LONGDESC="New rebasing of Hatari based on Mercurial upstream. Tries to be a shallow fork for easy upstreaming later on."

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"
PKG_USE_CMAKE="no"

configure_target() {
  :
}

make_target() {
  if [ "$ARCH" == "arm" ]; then
    CFLAGS="$CFLAGS -DNO_ASM -DARM -D__arm__ -DARM_ASM -DNOSSE -DARM_HARDFP"
  fi
  make -C .. -f Makefile.libretro
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp ../hatari_libretro.so $INSTALL/usr/lib/libretro/
}
