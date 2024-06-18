# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="zvbi"
PKG_VERSION="0.2.42"
PKG_SHA256="e7614a847ce7dd2c05f1db84d21dcf25085565932efb014f27107ae940884d7f"
PKG_LICENSE="GPL2"
PKG_SITE="https://github.com/zapping-vbi/zvbi"
PKG_URL="https://github.com/zapping-vbi/zvbi/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libpng zlib"
PKG_LONGDESC="Library to provide functions to capture and decode VBI data."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--disable-dvb \
                           --disable-bktr \
                           --disable-nls \
                           --disable-proxy \
                           --without-doxygen \
                           --without-x"
