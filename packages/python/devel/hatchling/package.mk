# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="hatchling"
PKG_VERSION="1.17.0"
PKG_SHA256="978e24b8ea205f57b250aaceb449c1ccf518b6590cfd86620ecf8829379e2c2e"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/pypa/hatch"
PKG_URL="https://github.com/pypa/hatch/archive/refs/tags/hatch-v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="pluggy:host python-pathspec:host trove-classifiers:host"
PKG_LONGDESC="Modern, extensible Python project management"
PKG_TOOLCHAIN="python"

pre_configure_host() {
  cd backend
}
