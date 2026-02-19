# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pybind11"
PKG_VERSION="3.0.2"
PKG_SHA256="2f20a0af0b921815e0e169ea7fec63909869323581b89d7de1553468553f6a2d"
PKG_LICENSE="pybind11"
PKG_SITE="https://github.com/pybind/pybind11"
PKG_URL="https://github.com/pybind/pybind11/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="scikit-build-core:host"
PKG_LONGDESC="Seamless operability between C++11 and Python"
PKG_TOOLCHAIN="python"

pre_configure_host() {
  cd ..
  rm -rf .${HOST_NAME}
}
