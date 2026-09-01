# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="lunasvg"
PKG_VERSION="3.5.0"
PKG_SHA256="1abf1472ee6c4d19797916e8cc3c2e4b628e0d81178ffac60bdb0d457e32c690"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/sammycage/lunasvg"
PKG_URL="https://github.com/sammycage/lunasvg/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A standalone SVG rendering and manipulation library."
PKG_BUILD_FLAGS="+pic -sysroot"
PKG_TOOLCHAIN="cmake"

# plutovg is bundled in-tree and built via add_subdirectory()
PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -DUSE_SYSTEM_PLUTOVG=OFF \
                       -DLUNASVG_BUILD_EXAMPLES=OFF \
                       -DLUNASVG_DISABLE_EXTERNAL_RESOURCES=ON"
