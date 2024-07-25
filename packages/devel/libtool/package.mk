# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libtool"
PKG_VERSION="2.5.3"
PKG_SHA256="898011232cc59b6b3bbbe321b60aba9db1ac11578ab61ed0df0299458146ae2e"
PKG_LICENSE="GPL"
PKG_SITE="https://www.gnu.org/software/libtool/"
PKG_URL="https://ftpmirror.gnu.org/libtool/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="ccache:host autoconf:host automake:host intltool:host"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A generic library support script."

PKG_CONFIGURE_OPTS_HOST="--enable-static --disable-shared"

post_unpack() {
  chmod u+w ${PKG_BUILD}/build-aux/ltmain.sh
}

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
  rm -rf ${INSTALL}/usr/share
}
