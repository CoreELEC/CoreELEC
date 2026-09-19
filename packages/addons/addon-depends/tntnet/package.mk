# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tntnet"
PKG_VERSION="694079d9bcd3b869e5342c85dacfe72f737a60f8"
PKG_SHA256="0d6435ae756bbbb89d129f038880be89e8c0349de36b0ce00809554ead86a93a"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://github.com/maekitalo/tntnet"
PKG_URL="${PKG_SITE}/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="cxxtools:host zlib:host"
PKG_DEPENDS_TARGET="toolchain tntnet:host libtool cxxtools zlib"
PKG_LONGDESC="A web application server for C++."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_HOST="--disable-unittest \
                         --disable-server \
                         --enable-sdk \
                         --disable-demos"

PKG_CONFIGURE_OPTS_TARGET="--disable-unittest \
                           --with-sysroot=${SYSROOT_PREFIX} \
                           --disable-server \
                           --disable-sdk \
                           --disable-demos"

post_configure_target() {
  libtool_remove_rpath libtool
}

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
  rm -rf ${INSTALL}/usr/share
}
