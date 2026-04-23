# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pybind11"
PKG_VERSION="3.0.4"
PKG_SHA256="74b6a2c2b4573a400cafb6ecbf60c98df300cd3d0041296b913d02b2cbbb2676"
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
