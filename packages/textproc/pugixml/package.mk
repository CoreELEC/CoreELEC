# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pugixml"
PKG_VERSION="1.15"
PKG_SHA256="b39647064d9e28297a34278bfb897092bf33b7c487906ddfc094c9e8868bddcb"
PKG_LICENSE="MIT"
PKG_SITE="https://pugixml.org/"
PKG_URL="https://github.com/zeux/pugixml/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="toolchain:host"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Light-weight, simple and fast XML parser for C++ with XPath support."
PKG_BUILD_FLAGS="+pic"
