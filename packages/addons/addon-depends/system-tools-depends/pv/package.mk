# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pv"
PKG_VERSION="1.10.4"
PKG_SHA256="7e994f9b8645820a289288468051575adf09a1e688d8e4d8fa3ecc48778bf7c9"
PKG_LICENSE="GNU"
PKG_SITE="http://www.ivarch.com/programs/pv.shtml"
PKG_URL="http://www.ivarch.com/programs/sources/pv-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Pipe Viewer can be inserted into any normal pipeline between two processes."
PKG_BUILD_FLAGS="-sysroot -cfg-libs"
