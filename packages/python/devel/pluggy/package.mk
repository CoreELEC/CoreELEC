# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pluggy"
PKG_VERSION="1.7.0"
PKG_SHA256="e46514f7995ac7e4f0ee351de4e7f55b23e7db66f484b7b1a5857beefbd09789"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/pytest-dev/pluggy"
PKG_URL="https://github.com/pytest-dev/pluggy/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="Python3:host setuptools:host setuptools-scm:host"
PKG_LONGDESC="A minimalist production ready plugin system"
PKG_TOOLCHAIN="python"

pre_configure_host() {
  export SETUPTOOLS_SCM_PRETEND_VERSION=${PKG_VERSION}
}
