# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="miniupnpc"
PKG_VERSION="2.2.6"
PKG_SHA256="37fcd91953508c3e62d6964bb8ffbc5d47f3e13481fa54e6214fcc68704c66f1"
PKG_LICENSE="BSD"
PKG_SITE="http://miniupnp.free.fr"
PKG_URL="http://miniupnp.free.fr/files/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="The MiniUPnP project offers software which supports the UPnP Internet Gateway Device (IGD) specifications"

PKG_CMAKE_OPTS_TARGET="-DUPNPC_BUILD_SHARED=OFF -DUPNPC_BUILD_STATIC=ON"
