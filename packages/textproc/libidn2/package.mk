# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libidn2"
PKG_VERSION="2.3.8"
PKG_SHA256="f557911bf6171621e1f72ff35f5b1825bb35b52ed45325dcdee931e5d3c0787a"
PKG_LICENSE="LGPL-3.0-or-later"
PKG_SITE="https://www.gnu.org/software/libidn/"
PKG_URL="https://ftpmirror.gnu.org/gnu/libidn/libidn2-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="autotools:host"
PKG_DEPENDS_TARGET="autotools:host gcc:host"
PKG_LONGDESC="Free software implementation of IDNA2008, Punycode and TR46."

PKG_CONFIGURE_OPTS_COMMON="--disable-doc \
                           --enable-shared \
                           --disable-static"

PKG_CONFIGURE_OPTS_HOST="${PKG_CONFIGURE_OPTS_COMMON}"
PKG_CONFIGURE_OPTS_TARGET="${PKG_CONFIGURE_OPTS_COMMON}"

post_makeinstall_target() {
  safe_remove ${INSTALL}/usr/bin
}
