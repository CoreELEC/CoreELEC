# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="md4c"
PKG_VERSION="0.5.3"
PKG_SHA256="353c346f376b87c954a13f3415ede2d51264cc61dc5abcd38ff1d2aa0d059b9e"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/mity/md4c"
PKG_URL="https://github.com/mity/md4c/archive/refs/tags/release-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="C Markdown parser. Fast. SAX-like interface. Compliant to CommonMark specification."
PKG_BUILD_FLAGS="-sysroot"

PKG_CMAKE_OPTS_TARGET="-DBUILD_MD2HTML_EXECUTABLE=OFF \
                       -DBUILD_SHARED_LIBS=ON"
