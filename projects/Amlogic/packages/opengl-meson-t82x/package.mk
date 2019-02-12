# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="opengl-meson-t82x"
PKG_VERSION="3b21f97"
PKG_SHA256="bb97c408a7b802592cb952c8ac117e810635366ab4447c5a335511fc279f3852"
PKG_LICENSE="nonfree"
PKG_SITE="https://github.com/kszaq/opengl-meson-t82x"
PKG_URL="https://github.com/kszaq/opengl-meson-t82x/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain libhybris opengl-meson"
PKG_SOURCE_DIR="$PKG_NAME-$PKG_VERSION*"
PKG_LONGDESC="OpenGL ES pre-compiled libraries for Mali GPUs. The libraries were extracted from Khadas VIM2 Android firmware."
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/system
    cp -a system/* $INSTALL/system
}

post_install() {
  enable_service unbind-console.service
}
