# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="msgpack-c"
PKG_VERSION="021da5d4b590a8bdf3d720d57b02196566753264"
PKG_SHA256=""
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/msgpack/msgpack-c"
PKG_URL="https://github.com/msgpack/msgpack-c/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Its like JSON but fast and small."

PKG_CMAKE_OPTS_TARGET="-DMSGPACK_ENABLE_SHARED=OFF \
                       -DMSGPACK_ENABLE_STATIC=ON"
