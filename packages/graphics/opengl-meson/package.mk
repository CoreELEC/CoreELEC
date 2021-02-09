# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="opengl-meson"
PKG_VERSION="gxbb-r5p1-01rel0"
PKG_SHA256="1a24a898ada066e85077a46bde7e15b3a4219a9b2e46f12d637c4f336ffcf2ca"
#PKG_VERSION="gxbb-r6p1-01rel0"
#PKG_SHA256="8fca82b19eeac5ed2f41bd4c9433f1f4599eb38d6f00e16ed3336d436540cf91"
PKG_LICENSE="nonfree"
PKG_SITE="http://openlinux.amlogic.com:8000/download/ARM/filesystem/"
PKG_URL="http://sources.libreelec.tv/mirror/opengl-meson/opengl-meson-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="OpenGL ES pre-compiled libraries for Mali GPUs found in Amlogic Meson SoCs."
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/
	cp -PR usr/lib/libMali.so $INSTALL/usr/lib/
    
  mkdir -p $SYSROOT_PREFIX/usr/lib
	cp -PR usr/lib/libMali.so $SYSROOT_PREFIX/usr/lib

   ln -sf /var/lib/libEGL.so $INSTALL/usr/lib/libEGL.so
   ln -sf /usr/lib/libEGL.so $INSTALL/usr/lib/libEGL.so.1
   ln -sf /usr/lib/libEGL.so $INSTALL/usr/lib/libEGL.so.1.0.0
   ln -sf /var/lib/libGLESv1_CM.so $INSTALL/usr/lib/libGLESv1_CM.so
   ln -sf /usr/lib/libGLESv1_CM.so $INSTALL/usr/lib/libGLESv1_CM.so.1
   ln -sf /usr/lib/libGLESv1_CM.so $INSTALL/usr/lib/libGLESv1_CM.so.1.0.1
   ln -sf /var/lib/libGLESv2.so $INSTALL/usr/lib/libGLESv2.so
   ln -sf /usr/lib/libGLESv2.so $INSTALL/usr/lib/libGLESv2.so.2
   ln -sf /usr/lib/libGLESv2.so $INSTALL/usr/lib/libGLESv2.so.2.0.0
   ln -sf /var/lib/libGLESv3.so $INSTALL/usr/lib/libGLESv3.so
   ln -sf /usr/lib/libGLESv3.so $INSTALL/usr/lib/libGLESv3.so.3
   ln -sf /usr/lib/libGLESv3.so $INSTALL/usr/lib/libGLESv3.so.3.0.0

  mkdir -p $INSTALL/usr/sbin
    cp $PKG_DIR/scripts/libmali-overlay-setup $INSTALL/usr/sbin
}

post_install() {
  enable_service libmali.service
}
