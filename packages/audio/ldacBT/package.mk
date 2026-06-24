# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ldacBT"
PKG_VERSION="f0c878e19c384ad2284489d28e6da4ea049aff85"
PKG_SHA256="d38e9d689fe5c287a7a2317b979c6ec2b44db7d481a2c3971edf4f90be05ec54"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/EHfive/ldacBT"
PKG_URL="https://github.com/EHfive/ldacBT/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_DEPENDS_UNPACK="libldac"
PKG_LONGDESC="LDAC Bluetooth encoder library (build tools)"

PKG_CMAKE_OPTS_TARGET="-DLDAC_SOFT_FLOAT=OFF"

post_unpack() {
  rm -rf ${PKG_BUILD}/libldac
  ln -sf $(get_build_dir libldac) ${PKG_BUILD}/libldac
}
