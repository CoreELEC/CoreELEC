# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nghttp2"
PKG_VERSION="1.55.1"
PKG_SHA256="19490b7c8c2ded1cf7c3e3a54ef4304e3a7876ae2d950d60a81d0dc6053be419"
PKG_LICENSE="MIT"
PKG_SITE="http://www.linuxfromscratch.org/blfs/view/cvs/basicnet/nghttp2.html"
PKG_URL="https://github.com/nghttp2/nghttp2/releases/download/v${PKG_VERSION}/nghttp2-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="nghttp2 is an implementation of HTTP/2 and its header compression algorithm, HPACK."

PKG_CMAKE_OPTS_TARGET="-DENABLE_DOC=OFF \
                       -DENABLE_FAILMALLOC=OFF \
                       -DENABLE_LIB_ONLY=ON \
                       -DENABLE_SHARED_LIB=ON \
                       -DENABLE_STATIC_LIB=OFF"

post_makeinstall_target() {
  rm -r "${INSTALL}/usr/share"
}
