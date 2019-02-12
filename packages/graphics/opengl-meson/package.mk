# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="opengl-meson"
PKG_ARCH="arm"
PKG_LICENSE="nonfree"
PKG_SITE="http://openlinux.amlogic.com:8000/download/ARM/filesystem/"
PKG_VERSION="8-r7p0-01rel0-armhf"
PKG_SHA256="d79526d849c2cc0c513335b7f78b9ea568c04d241af3a8c4636298a3ae4c9211"
PKG_URL="$DISTRO_SRC/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="OpenGL ES pre-compiled libraries for Mali GPUs found in Amlogic Meson SoCs."
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/
    cp -PR usr/lib/libMali.so $INSTALL/usr/lib/

   ln -sf /var/lib/libEGL.so $INSTALL/usr/lib/libEGL.so
   ln -sf /usr/lib/libEGL.so $INSTALL/usr/lib/libEGL.so.1
   ln -sf /var/lib/libGLESv1_CM.so $INSTALL/usr/lib/libGLESv1_CM.so
   ln -sf /usr/lib/libGLESv1_CM.so $INSTALL/usr/lib/libGLESv1_CM.so.1
   ln -sf /var/lib/libGLESv2.so $INSTALL/usr/lib/libGLESv2.so
   ln -sf /usr/lib/libGLESv2.so $INSTALL/usr/lib/libGLESv2.so.2
   ln -sf /var/lib/libGLESv3.so $INSTALL/usr/lib/libGLESv3.so
   ln -sf /usr/lib/libGLESv3.so $INSTALL/usr/lib/libGLESv3.so.3

  mkdir -p $INSTALL/usr/sbin
    cp $PKG_DIR/scripts/libmali-overlay-setup $INSTALL/usr/sbin
}

post_install() {
  enable_service libmali.service
}
