# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libva-utils"
PKG_VERSION="2.20.1"
PKG_SHA256="97413a7ec27ec479b97ffc7ab8acebe053615224a4b051602859cf9f88e4e889"
PKG_LICENSE="GPL"
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
