# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mpg123"
PKG_VERSION="1.32.6"
PKG_SHA256="ccdd1d0abc31d73d8b435fc658c79049d0a905b30669b6a42a03ad169dc609e6"
PKG_LICENSE="LGPLv2"
PKG_SITE="https://www.mpg123.org/"
PKG_URL="https://downloads.sourceforge.net/sourceforge/mpg123/mpg123-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain alsa-lib"
PKG_LONGDESC="A console based real time MPEG Audio Player for Layer 1, 2 and 3."
PKG_BUILD_FLAGS="-sysroot +pic"

PKG_CONFIGURE_OPTS_TARGET="--disable-shared \
                           --enable-static"
