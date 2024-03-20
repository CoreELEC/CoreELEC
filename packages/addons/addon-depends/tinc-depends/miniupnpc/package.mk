# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="miniupnpc"
PKG_VERSION="2.2.7"
PKG_SHA256="b0c3a27056840fd0ec9328a5a9bac3dc5e0ec6d2e8733349cf577b0aa1e70ac1"
PKG_LICENSE="BSD"
PKG_SITE="http://miniupnp.free.fr"
PKG_URL="http://miniupnp.free.fr/files/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="The MiniUPnP project offers software which supports the UPnP Internet Gateway Device (IGD) specifications"

PKG_CMAKE_OPTS_TARGET="-DUPNPC_BUILD_SHARED=OFF -DUPNPC_BUILD_STATIC=ON"
