# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mesa-demos"
PKG_VERSION="9.0.0"
PKG_SHA256="3046a3d26a7b051af7ebdd257a5f23bfeb160cad6ed952329cdff1e9f1ed496b"
PKG_ARCH="x86_64"
PKG_LICENSE="OSS"
PKG_SITE="https://www.mesa3d.org/"
PKG_URL="https://archive.mesa3d.org/demos/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libX11 mesa glu glew"
PKG_LONGDESC="Mesa 3D demos - installed are the well known glxinfo and glxgears."
PKG_BUILD_FLAGS="-sysroot"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp -P src/xdemos/glxdemo ${INSTALL}/usr/bin
    cp -P src/xdemos/glxgears ${INSTALL}/usr/bin
    cp -P src/xdemos/glxinfo ${INSTALL}/usr/bin
}
