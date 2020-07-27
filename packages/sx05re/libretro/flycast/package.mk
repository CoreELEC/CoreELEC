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

PKG_NAME="flycast"
PKG_VERSION="0e10e86ea9ca0f8655c98909da7a845e7643b36f"
PKG_SHA256="3a0e72a3c358520db2035c69f39fa1322ce024548dcc57afc1b2c822a47ac4a0"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/flycast"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain $OPENGLES"
PKG_LONGDESC="Flycast is a multiplatform Sega Dreamcast emulator "
PKG_TOOLCHAIN="make"
PKG_BUILD_FLAGS="-gold"

pre_configure_target() {
# Flycast defaults to -O3 but then CHD v5 do not seem to work on EmuELEC so we change it to -O2 to fix the issue
PKG_MAKE_OPTS_TARGET="ARCH=${ARCH} HAVE_OPENMP=1 GIT_VERSION=${PKG_VERSION:0:7} FORCE_GLES=1 SET_OPTIM=-O2 HAVE_LTCG=0"
}

pre_make_target() {
   case $PROJECT in
    Amlogic-ng)
      PKG_MAKE_OPTS_TARGET+=" platform=AMLG12B"
      ;;
    Amlogic)
      PKG_MAKE_OPTS_TARGET+=" platform=AMLGX"
    ;;  
  esac
  
 if [ "$DEVICE" == "OdroidGoAdvance" ]; then
	PKG_MAKE_OPTS_TARGET+=" platform=classic_armv8_a35"
 fi 
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp flycast_libretro.so $INSTALL/usr/lib/libretro/
}
