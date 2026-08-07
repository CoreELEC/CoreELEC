# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pybind11"
PKG_VERSION="3.1.0"
PKG_SHA256="ef712655692a2e9bf7bb7874c022564a45f91d847ddee987e720cd9e28849665"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://github.com/pybind/pybind11"
PKG_URL="https://github.com/pybind/pybind11/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="scikit-build-core:host"
PKG_LONGDESC="Seamless operability between C++11 and Python"
PKG_TOOLCHAIN="python"

pre_configure_host() {
  cd ..
  rm -rf .${HOST_NAME}
}
