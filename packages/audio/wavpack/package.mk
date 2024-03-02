# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wavpack"
PKG_VERSION="5.7.0"
PKG_SHA256="e81510fd9ec5f309f58d5de83e9af6c95e267a13753d7e0bbfe7b91273a88bee"
PKG_LICENSE="BSD"
PKG_SITE="https://www.wavpack.com"
PKG_URL="https://www.wavpack.com/wavpack-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libiconv"
PKG_LONGDESC="Audio compression format providing lossless, high-quality lossy and hybrid compression mode."

PKG_CMAKE_OPTS_TARGET="-DBUILD_TESTING=OFF \
                       -DCMAKE_INSTALL_INCLUDEDIR=include/wavpack \
                       -DWAVPACK_BUILD_PROGRAMS=OFF \
                       -DWAVPACK_ENABLE_ASM=OFF \
                       -DWAVPACK_INSTALL_DOCS=OFF"
