# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="jq"
PKG_VERSION="1.7.1"
PKG_SHA256="478c9ca129fd2e3443fe27314b455e211e0d8c60bc8ff7df703873deeee580c2"
PKG_LICENSE="MIT"
PKG_SITE="https://jqlang.github.io/jq/"
PKG_URL="https://github.com/jqlang/jq/releases/download/${PKG_NAME}-${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain oniguruma"
PKG_LONGDESC="Command-line JSON processor"
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET="--disable-shared \
                           --enable-static \
                           --disable-docs \
                           --disable-maintainer-mode \
                           --disable-valgrind"
