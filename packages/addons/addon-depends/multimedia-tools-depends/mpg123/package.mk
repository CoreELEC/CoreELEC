# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mpg123"
PKG_VERSION="1.33.7"
PKG_SHA256="31d0e35a4ca567ec9b5ebda6c3062bb4435d6d3eacd6ef0d95cadd7854dc03ee"
PKG_LICENSE="LGPL-2.1-only"
PKG_SITE="https://www.mpg123.org/"
PKG_URL="https://downloads.sourceforge.net/project/mpg123/mpg123/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain alsa-lib"
PKG_LONGDESC="A console based real time MPEG Audio Player for Layer 1, 2 and 3."
PKG_BUILD_FLAGS="-sysroot +pic"

PKG_CONFIGURE_OPTS_TARGET="--disable-shared \
                           --enable-static"
