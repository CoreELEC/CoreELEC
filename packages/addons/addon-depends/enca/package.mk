# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="enca"
PKG_VERSION="1.22"
PKG_SHA256="95a70dd21198e6427d77a1d79721f4f87dd8bd07fdefe71a2062c6f41eee39da"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/Project-OSS-Revival/enca/"
PKG_URL="https://github.com/Project-OSS-Revival/enca/releases/download/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Enca detects the encoding of text files, on the basis of knowledge of their language."
PKG_BUILD_FLAGS="+pic"

PKG_MAKEINSTALL_OPTS_TARGET="-C lib"
PKG_CONFIGURE_OPTS_TARGET="ac_cv_file__dev_random=yes \
                           ac_cv_file__dev_urandom=no \
                           ac_cv_file__dev_srandom=no \
                           ac_cv_file__dev_arandom=no \
                           CPPFLAGS="-I${SYSROOT_PREFIX}/usr/include" \
                           --disable-shared \
                           --enable-static \
                           --disable-external \
                           --without-librecode \
                           --disable-rpath \
                           --with-gnu-ld"

pre_make_target() {
  make CC="${HOST_CC}" \
       CPPFLAGS="${HOST_CPPFLAGS}" \
       CFLAGS="${HOST_CFLAGS}" \
       LDFLAGS="${HOST_LDFLAGS}" \
       -C tools
}

post_makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/pkgconfig
    cp enca.pc ${SYSROOT_PREFIX}/usr/lib/pkgconfig
}
