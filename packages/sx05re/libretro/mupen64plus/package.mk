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

PKG_NAME="mupen64plus"
PKG_VERSION="ab8134ac90a567581df6de4fc427dd67bfad1b17"
PKG_SHA256="98e197cdcac64c0e08eda91a6d63b637c3f151066bede25766e62bc1a59552a0"
PKG_REV="1"
PKG_ARCH="arm i386 x86_64"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/mupen64plus-libretro"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain nasm:host $OPENGLES"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="mupen64plus + RSP-HLE + GLideN64 + libretro"
PKG_LONGDESC="mupen64plus + RSP-HLE + GLideN64 + libretro"
PKG_TOOLCHAIN="make"
PKG_BUILD_FLAGS="-lto"

pre_configure_target() {
  
   case $PROJECT in
    RPi|Gamegirl|Slice)
      CFLAGS="$CFLAGS -I$SYSROOT_PREFIX/usr/include/interface/vcos/pthreads \
	              -I$SYSROOT_PREFIX/usr/include/interface/vmcs_host/linux"
      PKG_MAKE_OPTS_TARGET=" platform=rpi GLES=1 FORCE_GLES=1 WITH_DYNAREC=arm"
      ;;
    RPi2|Slice3)
      CFLAGS="$CFLAGS -I$SYSROOT_PREFIX/usr/include/interface/vcos/pthreads \
                      -I$SYSROOT_PREFIX/usr/include/interface/vmcs_host/linux"
      PKG_MAKE_OPTS_TARGET=" platform=rpi2 GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
      ;;
    imx6|Amlogic*)
      CFLAGS="$CFLAGS -DLINUX -DEGL_API_FB"
      CPPFLAGS="$CPPFLAGS -DLINUX -DEGL_API_FB"
      PKG_MAKE_OPTS_TARGET=" platform=unix GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
      ;;
    Generic)
	  PKG_MAKE_OPTS_TARGET=""
      ;;
    OdroidC1)
      PKG_MAKE_OPTS_TARGET=" platform=odroid BOARD=ODROID-C1 GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
      ;;
    OdroidXU3)
      PKG_MAKE_OPTS_TARGET=" platform=odroid BOARD=ODROID-XU3 GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
      ;;
    ROCK960)
      PKG_MAKE_OPTS_TARGET=" platform=unix-gles GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
      ;;
    *)
      PKG_MAKE_OPTS_TARGET=" platform=unix-gles GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
      ;;
  esac
 
 if [ "$DEVICE" == "OdroidGoAdvance" ]; then 
	CFLAGS="$CFLAGS -DLINUX -DEGL_API_FB"
    CPPFLAGS="$CPPFLAGS -DLINUX -DEGL_API_FB"
    PKG_MAKE_OPTS_TARGET=" platform=unix GLES=1 FORCE_GLES=1 HAVE_NEON=1 WITH_DYNAREC=arm"
 fi
  
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mupen64plus_libretro.so $INSTALL/usr/lib/libretro/
}
