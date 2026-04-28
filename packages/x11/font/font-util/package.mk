# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="font-util"
PKG_VERSION="1.4.2"
PKG_SHA256="02e4f8afdcf03cc8372ca9c37aa104b1e36b47722dbc79531be08f0a4c622999"
PKG_LICENSE="OSS"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/font/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros"
PKG_DEPENDS_HOST="util-macros"
PKG_LONGDESC="X.org font utilities."

PKG_CONFIGURE_OPTS_TARGET="--with-fontrootdir=/usr/share/fonts \
                           --with-mapdir=/usr/share/fonts/util"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
}
