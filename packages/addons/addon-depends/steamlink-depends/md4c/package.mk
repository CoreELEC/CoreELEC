# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="md4c"
PKG_VERSION="0.6.0"
PKG_SHA256="4d151298125a81da3b2efa2e0eed8bdb7a9318569804e4fa4d7a2375ab83ef70"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/mity/md4c"
PKG_URL="https://github.com/mity/md4c/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="C Markdown parser. Fast. SAX-like interface. Compliant to CommonMark specification."
PKG_BUILD_FLAGS="-sysroot"

PKG_CMAKE_OPTS_TARGET="-DBUILD_MD2HTML_EXECUTABLE=OFF \
                       -DBUILD_SHARED_LIBS=ON"
