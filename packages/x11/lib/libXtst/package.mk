# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXtst"
PKG_VERSION="1.2.6"
PKG_SHA256="f94f0b37c07d80d877f3eda2c2405cc7e62aa9842f6310354aa349e3d66a4fe5"
PKG_LICENSE="MIT"
PKG_SITE="http://www.X.org"
PKG_URL="http://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libXext libXi libX11"
PKG_LONGDESC="The Xtst Library"

PKG_CONFIGURE_OPTS_TARGET="--with-gnu-ld --without-xmlto"

post_configure_target() {
  libtool_remove_rpath libtool
}
