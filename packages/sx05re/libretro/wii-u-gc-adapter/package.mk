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

PKG_NAME="wii-u-gc-adapter"
PKG_VERSION="a99223736e188822f8c6743016836ae4a4529aa1"
PKG_SHA256="09e1a591ffeccab3d765cd01524ea7d27e919ba8d90ed58edbc3c80216b26cc3"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="Non-commercial"
PKG_SITE="https://github.com/ToadKing/wii-u-gc-adapter"
PKG_URL="https://github.com/ToadKing/wii-u-gc-adapter/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION=""
PKG_SHORTDESC="Tool for using the Wii U GameCube Adapter on Linux"
PKG_LONGDESC="Tool for using the Wii U GameCube Adapter on Linux"

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  cp wii-u-gc-adapter $INSTALL/usr/bin/
}
