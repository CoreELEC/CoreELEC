# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pugixml"
PKG_VERSION="1.14"
PKG_SHA256="610f98375424b5614754a6f34a491adbddaaec074e9044577d965160ec103d2e"
PKG_LICENSE="MIT"
PKG_SITE="https://pugixml.org/"
PKG_URL="https://github.com/zeux/pugixml/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="toolchain:host"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Light-weight, simple and fast XML parser for C++ with XPath support."
PKG_BUILD_FLAGS="+pic"
