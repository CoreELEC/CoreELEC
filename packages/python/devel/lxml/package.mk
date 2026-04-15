# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="lxml"
PKG_VERSION="6.0.4"
PKG_SHA256="4137516be2a90775f99d8ef80ec0283f8d78b5d8bd4630ff20163b72e7e9abf2"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://lxml.de"
PKG_URL="https://github.com/lxml/lxml/releases/download/${PKG_NAME}-${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="Python3:host libxml2:host libxslt:host setuptools:host"
PKG_LONGDESC="The lxml XML toolkit for Python"
PKG_TOOLCHAIN="python"
