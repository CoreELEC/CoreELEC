# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="hatchling"
PKG_VERSION="1.17.1"
PKG_SHA256="2f3c36c0a1e39d955f2db83c2a230529a198a454c9627ed45b4701e5bbcb088c"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/pypa/hatch"
PKG_URL="https://github.com/pypa/hatch/archive/refs/tags/hatch-v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="pluggy:host python-pathspec:host trove-classifiers:host"
PKG_LONGDESC="Modern, extensible Python project management"
PKG_TOOLCHAIN="python"

pre_configure_host() {
  cd backend
}
