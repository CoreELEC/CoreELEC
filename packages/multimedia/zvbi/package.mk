# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="zvbi"
PKG_VERSION="0.2.44"
PKG_SHA256="bca620ab670328ad732d161e4ce8d9d9fc832533cb7440e98c50e112b805ac5e"
PKG_LICENSE="LGPL-2.0-or-later"
PKG_SITE="https://github.com/zapping-vbi/zvbi"
PKG_URL="https://github.com/zapping-vbi/zvbi/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libpng zlib"
PKG_LONGDESC="Library to provide functions to capture and decode VBI data."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--disable-dvb \
                           --disable-bktr \
                           --disable-examples \
                           --disable-nls \
                           --disable-proxy \
                           --disable-tests \
                           --disable-examples \
                           --without-doxygen \
                           --without-x"
