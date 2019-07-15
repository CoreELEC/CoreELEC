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

PKG_NAME="parallel-n64"
PKG_VERSION="68d89c77c37cb6d3da05245f75ea6f949096da96"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/parallel-n64"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Optimized/rewritten Nintendo 64 emulator made specifically for Libretro. Originally based on Mupen64 Plus."
PKG_LONGDESC="Optimized/rewritten Nintendo 64 emulator made specifically for Libretro. Originally based on Mupen64 Plus."
PKG_TOOLCHAIN="make"
PKG_BUILD_FLAGS="+verbose"

#pre_configure_target() { 
#sed -i -e "s|uname -a|echo armv|" \
#         -e "s|uname -m|echo armv|" \
#         -e "s|LIBS =|LIBS = -lm|g" \
#    Makefile
#sed -i -e "s|void rglBlendFuncSeparate(GLenum sfactor, GLenum dfactor)|rglBlendFuncSeparate(GLenum sfactor, GLenum dfactor)|" $PKG_BUILD/libretro-common/glsm/glsm.c

#sed -i -e "s|void rglBlendFuncSeparate(GLenum sfactor, GLenum dfactor)|rglBlendFuncSeparate(GLenum sfactor, GLenum dfactor)|" Makefile

# echo "Default LDFAGS: $CXXFLAGS"
#CFLAGS="-mabi=aapcs-linux -Wno-psabi -Wa,-mno-warn-deprecated -mfloat-abi=hard -fomit-frame-pointer -Wall -pipe -O2"
#CXXFLAGS="-mabi=aapcs-linux -Wno-psabi -Wa,-mno-warn-deprecated -mfloat-abi=hard -fomit-frame-pointer -Wall -pipe -O2"
#LDFLAGS=""
#}

#if [ ${PROJECT} = "Amlogic" ]; then
#  PKG_PATCH_DIRS="${PROJECT}"
#fi

make_target() {
  make platform=imx6 DYNAREC=$ARCH ARCH=$ARCH
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp parallel_n64_libretro.so $INSTALL/usr/lib/libretro/
}
