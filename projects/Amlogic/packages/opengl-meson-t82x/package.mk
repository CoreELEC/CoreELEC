# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017-2019 Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="opengl-meson-t82x"
PKG_VERSION="d3e7cfed0c2b92640d874bb8c8c16d7ed832fdd4"
PKG_SHA256="b4c9b2319da9b1d4aec4e401ee2bf7853303d75df4a192da7bd1e18353ec1334"
PKG_LICENSE="nonfree"
PKG_SITE="https://github.com/CoreELEC/opengl-meson-t82x"
PKG_URL="https://github.com/CoreELEC/opengl-meson-t82x/archive/$PKG_VERSION.tar.gz"
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
