# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2014 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nettle"
PKG_VERSION="4.0"
PKG_SHA256="3addbc00da01846b232fb3bc453538ea5468da43033f21bb345cb1e9073f5094"
PKG_LICENSE="GPL2"
PKG_SITE="http://www.lysator.liu.se/~nisse/nettle"
PKG_URL="https://ftp.gnu.org/gnu/nettle/nettle-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="autotools:host gmp:host"
PKG_DEPENDS_TARGET="autotools:host gcc:host gmp"
PKG_LONGDESC="A low-level cryptographic library."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_COMMON="--disable-documentation \
                           --disable-openssl"

PKG_CONFIGURE_OPTS_HOST="${PKG_CONFIGURE_OPTS_COMMON}"
PKG_CONFIGURE_OPTS_TARGET="${PKG_CONFIGURE_OPTS_COMMON}"

if target_has_feature neon; then
  PKG_CONFIGURE_OPTS_TARGET+=" --enable-arm-neon"
fi

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
}
