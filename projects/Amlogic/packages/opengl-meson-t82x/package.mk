# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="opengl-meson-t82x"
PKG_VERSION="3b21f97"
PKG_SHA256="f2f928b77e87d2c9d42583f346a95b5ec1bc572e0cb8e81f3bf9dfe1de0ef1e6"
PKG_ARCH="arm"
PKG_LICENSE="nonfree"
PKG_SITE="https://github.com/kszaq/opengl-meson-t82x"
PKG_URL="https://github.com/kszaq/opengl-meson-t82x/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain libhybris"
PKG_SOURCE_DIR="$PKG_NAME-$PKG_VERSION*"
PKG_SECTION="graphics"
PKG_SHORTDESC="opengl-meson: OpenGL ES pre-compiled libraries for Mali GPUs found in Amlogic Meson SoCs"
PKG_LONGDESC="opengl-meson: OpenGL ES pre-compiled libraries for Mali GPUs found in Amlogic Meson SoCs. The libraries were extracted from Khadas VIM2 Android firmware."
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/system
    cp -a system/* $INSTALL/system
}

post_install() {
  enable_service unbind-console.service
}
