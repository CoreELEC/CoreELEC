# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libva-utils"
PKG_VERSION="2.24.0"
PKG_SHA256="bf959a1ced3cde8176a7ff50ad358ee98e93301ac068581a8b2617c5b83afcb3"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/01org/libva-utils"
PKG_URL="https://github.com/intel/libva-utils/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Libva-utils is a collection of tests for VA-API (VIdeo Acceleration API)"

if [ "${DISPLAYSERVER}" = "x11" ]; then
  PKG_DEPENDS_TARGET="toolchain libva libdrm libX11"
  DISPLAYSERVER_LIBVA="-Dx11=true"
else
  PKG_DEPENDS_TARGET="toolchain libva libdrm"
  DISPLAYSERVER_LIBVA="-Dx11=false"
fi

PKG_MESON_OPTS_TARGET="-Ddrm=true \
                       ${DISPLAYSERVER_LIBVA} \
                       -Dwayland=false \
                       -Dtests=false"
