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

PKG_NAME="mrboom"
PKG_VERSION="d6a1a24b7e47eba1c7b29d3d48d3148b54e1fbf8"
PKG_SHA256="07f580bc441e6530e819565a43d0d3a25f34726e32c67d939213dcc6254221ee"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/libretro/mrboom-libretro"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Mr.Boom is a 8 players Bomberman clone for RetroArch/Libretro"
PKG_LONGDESC="Mr.Boom is a 8 players Bomberman clone for RetroArch/Libretro"

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"

pre_configure_target() {
 PKG_MAKE_OPTS_TARGET="HAVE_NEON=1"
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mrboom_libretro.so $INSTALL/usr/lib/libretro/
}
