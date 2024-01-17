# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libevent"
PKG_VERSION="2.1.12"
PKG_SHA256="92e6de1be9ec176428fd2367677e61ceffc2ee1cb119035037a27d346b0403bb"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://libevent.org/"
PKG_URL="https://github.com/libevent/libevent/releases/download/release-${PKG_VERSION}-stable/libevent-${PKG_VERSION}-stable.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libevent – an event notification library"

PKG_CMAKE_OPTS_TARGET="-DEVENT__LIBRARY_TYPE=STATIC \
                       -DEVENT__DISABLE_DEBUG_MODE=ON \
                       -DEVENT__DISABLE_MM_REPLACEMENT=ON \
                       -DEVENT__DISABLE_THREAD_SUPPORT=ON \
                       -DEVENT__DISABLE_OPENSSL=ON \
                       -DEVENT__DISABLE_BENCHMARK=ON \
                       -DEVENT__DISABLE_TESTS=ON \
                       -DEVENT__DISABLE_REGRESS=ON \
                       -DEVENT__DISABLE_SAMPLES=ON"
