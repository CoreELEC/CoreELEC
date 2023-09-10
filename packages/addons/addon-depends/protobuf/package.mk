# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="protobuf"
PKG_VERSION="21.12"
PKG_SHA256="4eab9b524aa5913c6fffb20b2a8abf5ef7f95a80bc0701f3a6dbb4c607f73460"
PKG_LICENSE="OSS"
PKG_SITE="https://developers.google.com/protocol-buffers/"
PKG_URL="https://github.com/google/${PKG_NAME}/releases/download/v${PKG_VERSION}/${PKG_NAME}-cpp-3.${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="toolchain:host zlib:host"
PKG_DEPENDS_TARGET="toolchain zlib protobuf:host"
PKG_LONGDESC="Protocol Buffers for Google's data interchange format."

PKG_CMAKE_OPTS_HOST="-DCMAKE_NO_SYSTEM_FROM_IMPORTED=1 \
                     -DBUILD_SHARED_LIBS=0 \
                     -Dprotobuf_BUILD_TESTS=0 \
                     -Dprotobuf_BUILD_EXAMPLES=0 \
                     -Dprotobuf_WITH_ZLIB=1"

PKG_CMAKE_OPTS_TARGET="${PKG_CMAKE_OPTS_HOST}"

configure_package() {
  PKG_CMAKE_SCRIPT="${PKG_BUILD}/cmake/CMakeLists.txt"
}

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin

  # HACK: we have protoc in ${TOOLCHAIN}/bin but it seems
  # the one from sysroot prefix is picked when building hyperion. remove it!
  rm -f ${SYSROOT_PREFIX}/usr/bin/protoc
}
