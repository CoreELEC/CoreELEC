# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pugixml"
PKG_VERSION="1.16"
PKG_SHA256="357bcab8877dc9943f355d3a72daba1b053238ba955f50fa81586afb65090219"
PKG_LICENSE="MIT"
PKG_SITE="https://pugixml.org/"
PKG_URL="https://github.com/zeux/pugixml/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="toolchain:host"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Light-weight, simple and fast XML parser for C++ with XPath support."
PKG_BUILD_FLAGS="+pic"
