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
PKG_VERSION="a628b32a4209d601c435462f02fef46d18aa422c"
PKG_SHA256="7f5fe0e3a59460865835b17ea6e040bf34e6681dd9aefbf37dd765c5f64daa26"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/flycast"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain $OPENGLES"
PKG_LONGDESC="Flycast is a multiplatform Sega Dreamcast emulator "
PKG_TOOLCHAIN="make"
PKG_BUILD_FLAGS="-gold"

if [ ${PROJECT} = "Amlogic-ng" ]; then
  PKG_PATCH_DIRS="${PROJECT}"
fi

PKG_MAKE_OPTS_TARGET="HAVE_OPENMP=0 GIT_VERSION=${PKG_VERSION:0:7} WITH_DYNAREC=$ARCH ARCH=arm"

pre_make_target() {
   export BUILD_SYSROOT=$SYSROOT_PREFIX

  if [ "$OPENGLES_SUPPORT" = "yes" ]; then
    PKG_MAKE_OPTS_TARGET+=" FORCE_GLES=1"
  fi

  case $PROJECT in
    Amlogic-ng)
      PKG_MAKE_OPTS_TARGET+=" platform=AMLG12B"
      ;;
    Amlogic)
      PKG_MAKE_OPTS_TARGET+=" platform=AMLGX"
    ;;  
  esac
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp flycast_libretro.so $INSTALL/usr/lib/libretro/
}
