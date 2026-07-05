# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ldacBT"
PKG_VERSION="2.0.2.6"
PKG_SHA256="fee05740e86ee66f4540486d92683ee8e8071119907b57ca762c7e5d943ecef0"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/EHfive/ldacBT"
PKG_URL="https://github.com/EHfive/ldacBT/releases/download/v${PKG_VERSION}/ldacBT-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="LDAC Bluetooth encoder library (build tools)"

PKG_CMAKE_OPTS_TARGET="-DLDAC_SOFT_FLOAT=OFF"
