# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="asio"
PKG_VERSION="1.38.2"
PKG_SHA256="1907e462b736f3b682e1539c6d7a8ececde66dcb25fe98c8386701c101a93388"
PKG_LICENSE="BSL-1.0"
PKG_SITE="http://think-async.com/Asio"
PKG_URL="https://github.com/chriskohlhoff/asio/archive/asio-${PKG_VERSION//./-}.zip"
PKG_SOURCE_DIR="asio-asio-${PKG_VERSION//./-}"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Asio C++ Library."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-sysroot -cfg-libs"

PKG_CONFIGURE_OPTS_TARGET="--without-boost --without-openssl"
