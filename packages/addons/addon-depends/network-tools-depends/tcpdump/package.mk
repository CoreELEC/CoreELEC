# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tcpdump"
PKG_VERSION="4.99.7"
PKG_SHA256="8be364e28d3b745ef1459b385cd2f4bc0e1ebad7a5d2ebdf70071d6c9b5b9a54"
PKG_SITE="https://www.tcpdump.org/"
PKG_URL="https://www.tcpdump.org/release/tcpdump-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libpcap libtirpc"
PKG_LONGDESC="A program that allows you to dump the traffic on a network."
PKG_BUILD_FLAGS="-sysroot -cfg-libs"
# use configure, not cmake. review cmake in future release.
PKG_TOOLCHAIN="configure"

PKG_CONFIGURE_OPTS_TARGET="--with-crypto=no"

pre_configure_target() {
  # When cross-compiling, configure can't set linux version
  # forcing it
  sed -i -e 's/ac_cv_linux_vers=unknown/ac_cv_linux_vers=2/' ../configure
  CFLAGS+=" -I${SYSROOT_PREFIX}/usr/include/tirpc"
  LDFLAGS+=" -ltirpc"
}

post_configure_target() {
  # discard native system includes
  sed -i "s%-I/usr/include%%g" Makefile
}
