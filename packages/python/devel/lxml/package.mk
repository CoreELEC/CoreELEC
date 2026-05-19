# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="lxml"
PKG_VERSION="6.1.1"
PKG_SHA256="ba96ae44888e0185281e937633a743ea90d5a196c6000f82565ebb0580012d40"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://lxml.de"
PKG_URL="https://github.com/lxml/lxml/releases/download/${PKG_NAME}-${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="Python3:host libxml2:host libxslt:host setuptools:host"
PKG_LONGDESC="The lxml XML toolkit for Python"
PKG_TOOLCHAIN="python"
