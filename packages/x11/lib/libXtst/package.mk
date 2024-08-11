# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXtst"
PKG_VERSION="1.2.5"
PKG_SHA256="b50d4c25b97009a744706c1039c598f4d8e64910c9fde381994e1cae235d9242"
PKG_LICENSE="OSS"
PKG_SITE="http://www.X.org"
PKG_URL="http://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libXext libXi libX11"
PKG_LONGDESC="The Xtst Library"

PKG_CONFIGURE_OPTS_TARGET="--with-gnu-ld --without-xmlto"

post_configure_target() {
  libtool_remove_rpath libtool
}
