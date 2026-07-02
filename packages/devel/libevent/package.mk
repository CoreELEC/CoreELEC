# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libevent"
PKG_VERSION="2.1.13"
PKG_SHA256="f7e9383b8c0baa81b687e5b5eecc01beefaf1b19b64151d95ed61647fe7a315c"
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
