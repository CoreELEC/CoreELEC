# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="jansson"
PKG_VERSION="2.15.0"
PKG_SHA256="070a629590723228dc3b744ae90e965a569efb9c535b3309b52e80e75d8eb3be"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/akheron/jansson"
PKG_URL="https://github.com/akheron/jansson/releases/download/v${PKG_VERSION}/jansson-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SHORTDESC="C library for encoding, decoding and manipulating JSON data"

PKG_CMAKE_OPTS_TARGET="-DJANSSON_BUILD_SHARED_LIBS=ON \
                       -DJANSSON_BUILD_DOCS=OFF \
                       -DJANSSON_COVERAGE=OFF \
                       -DJANSSON_EXAMPLES=OFF \
                       -DJANSSON_TEST_WITH_VALGRIND=OFF \
                       -DJANSSON_WITHOUT_TESTS=OFF \
                       -DUSE_DTOA=OFF"
