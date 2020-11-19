# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="libhybris"
PKG_VERSION="234a0480c887c29f8d6a895ac86a350e4e0fe05a"
PKG_SHA256="18551bee3d660bca744324b3fb5477bec1796f3b3fb8b15808fc43e248dd0ffb"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/dmikushin/libhybris"
PKG_URL="https://github.com/dmikushin/libhybris/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="$PKG_NAME-$PKG_VERSION*/hybris"
PKG_DEPENDS_TARGET="toolchain android-headers"
PKG_LONGDESC="Allows to run bionic-based HW adaptations in glibc systems - libs."
PKG_TOOLCHAIN="autotools"

HYBRIS_ARCH=$TARGET_ARCH
if [ "$HYBRIS_ARCH" = "aarch64" ]; then
  HYBRIS_ARCH=arm
fi

PKG_CONFIGURE_OPTS_TARGET="--enable-arch=$HYBRIS_ARCH \
                           --with-default-egl-platform=fbdev \
                           --with-android-headers=$BUILD/android-headers-25 \
                           --with-default-hybris-ld-library-path=/system/lib \
                           --enable-mali-quirks"

post_makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/malihybris
    mv $INSTALL/usr/lib/libEGL* $INSTALL/usr/lib/malihybris
    mv $INSTALL/usr/lib/libGLES* $INSTALL/usr/lib/malihybris
}
