# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="jq"
PKG_VERSION="1.8.2"
PKG_SHA256="71b8d6e8f5fe81f6c6d0d110e3892251f6ce76ed095abd315e26e6e1193af3af"
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
