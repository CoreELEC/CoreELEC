# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="spdlog"
PKG_VERSION="1.17.0"
PKG_SHA256="d8862955c6d74e5846b3f580b1605d2428b11d97a410d86e2fb13e857cd3a744"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/gabime/spdlog"
PKG_URL="https://github.com/gabime/spdlog/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libfmt"
PKG_LONGDESC="Very fast, header only, C++ logging library."
PKG_BUILD_FLAGS="+lto"

PKG_CMAKE_OPTS_TARGET="-DCMAKE_CXX_STANDARD=14 \
                       -DCMAKE_CXX_EXTENSIONS:BOOL=OFF \
                       -DSPDLOG_BUILD_SHARED=ON \
                       -DSPDLOG_FMT_EXTERNAL=ON \
                       -DSPDLOG_BUILD_EXAMPLE=OFF \
                       -DSPDLOG_BUILD_TESTS=OFF"
