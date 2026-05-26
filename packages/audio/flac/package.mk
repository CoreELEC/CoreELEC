# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="flac"
PKG_VERSION="1.5.0"
PKG_SHA256="f2c1c76592a82ffff8413ba3c4a1299b6c7ab06c734dee03fd88630485c2b920"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://xiph.org/flac/"
PKG_URL="https://downloads.xiph.org/releases/flac/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libogg"
PKG_LONGDESC="An Free Lossless Audio Codec."
PKG_TOOLCHAIN="autotools"
# flac-1.3.1 dont build with LTO support
PKG_BUILD_FLAGS="+pic -cfg-libs"

# package specific configure options
PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-rpath \
                           --disable-doxygen-docs \
                           --disable-thorough-tests \
                           --disable-cpplibs \
                           --disable-oggtest \
                           --with-ogg=${SYSROOT_PREFIX}/usr \
                           --with-gnu-ld"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
}
