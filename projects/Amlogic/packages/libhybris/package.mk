# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="libhybris"
PKG_VERSION="c21320b3c9bd043d05c95550a72b47f43016cfec"
PKG_SHA256="c71d8dc18b4784ad80c120869ec21b34fcdd1503be738f3ea0f51b9489001c60"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/kszaq/libhybris"
PKG_URL="https://github.com/kszaq/libhybris/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="$PKG_NAME-$PKG_VERSION*/hybris"
PKG_DEPENDS_TARGET="toolchain android-headers"
PKG_LONGDESC="Allows to run bionic-based HW adaptations in glibc systems - libs."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-arch=$TARGET_ARCH \
                           --with-default-egl-platform=fbdev \
                           --with-android-headers=$BUILD/android-headers-25 \
                           --with-default-hybris-ld-library-path=/system/lib \
                           --enable-mali-quirks"
                           
post_patch() {
  rm -rf $PKG_BUILD/compat
  rm -rf $PKG_BUILD/utils
  mv $PKG_BUILD/hybris/* $PKG_BUILD/
  rm -rf $PKG_BUILD/hybris
}

post_makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/malihybris
    mv $INSTALL/usr/lib/libEGL* $INSTALL/usr/lib/malihybris/
    mv $INSTALL/usr/lib/libGLES* $INSTALL/usr/lib/malihybris/
}
