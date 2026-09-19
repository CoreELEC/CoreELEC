# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="snappy"
PKG_VERSION="1.3.1"
PKG_SHA256="893f708a0bf4b5529d555ffcee390e940e932fcf90261f682604475a76cd0247"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://github.com/google/snappy"
PKG_URL="https://github.com/google/snappy/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A fast compressor/decompressor"
PKG_BUILD_FLAGS="-sysroot +pic"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -DSNAPPY_BUILD_BENCHMARKS=OFF \
                       -DSNAPPY_BUILD_TESTS=OFF"
