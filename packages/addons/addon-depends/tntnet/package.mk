# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tntnet"
PKG_VERSION="fdf69115cdac19e901fdc3bf6f7a06a30b2d87aa"
PKG_SHA256="5fe407cf35dfdf50fa22c6f46a767b0eef72304c0d60b5992bdcd8d3982aa4c1"
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
