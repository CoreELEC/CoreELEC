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

PKG_NAME="picodrive"
PKG_VERSION="56b24717adf4b0a43d548fad21abe3c8e1b99848"
PKG_SHA256="775ec23ecde0a3209abe99d5970e19ac7e3b3cac7aaa94d3037e86e545699004"
PKG_LICENSE="MAME"
PKG_SITE="https://github.com/irixxxx/picodrive"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain $PKG_NAME:host"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Libretro implementation of PicoDrive. (Sega Megadrive/Genesis/Sega Master System/Sega GameGear/Sega CD/32X)"
PKG_LONGDESC="This is yet another Megadrive / Genesis / Sega CD / Mega CD / 32X / SMS emulator, which was written having ARM-based handheld devices in mind (such as smartphones and handheld consoles like GP2X and Pandora), but also runs on non-ARM little-endian hardware too."
GET_HANDLER_SUPPORT="git"
PKG_BUILD_FLAGS="-gold"
PKG_GIT_BRANCH="libretro"

PKG_IS_ADDON="no"
PKG_AUTORECONF="no"


configure_host() {
  :
}

make_host() {
  if [ "$ARCH" == "arm" ]; then
    make -C ../cpu/cyclone CONFIG_FILE=../cyclone_config.h
  fi
}

makeinstall_host() {
  :
}

configure_target() {
  :
}

make_target() {
  if [ "$ARCH" == "arm" ]; then
    make -C .. -f Makefile.libretro platform=armv6
  elif [ "$ARCH" == "aarch64" ]; then
    cd $PKG_BUILD
    $PKG_BUILD/configure --platform=generic
    make -f Makefile.libretro
  else
    make -C .. -f Makefile.libretro
  fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp picodrive_libretro.so $INSTALL/usr/lib/libretro/
}