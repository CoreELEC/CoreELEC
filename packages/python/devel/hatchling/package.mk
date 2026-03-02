# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="hatchling"
PKG_VERSION="1.16.5"
PKG_SHA256="a1361a46772c7589c61c08968ebf081c69df8539d204a31fa7729733f9c1915a"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/pypa/hatch"
PKG_URL="https://github.com/pypa/hatch/archive/refs/tags/hatch-v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="pluggy:host python-pathspec:host trove-classifiers:host"
PKG_LONGDESC="Modern, extensible Python project management"
PKG_TOOLCHAIN="python"

pre_configure_host() {
  cd backend
}
