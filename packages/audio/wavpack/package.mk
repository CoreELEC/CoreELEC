# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wavpack"
PKG_VERSION="5.9.0"
PKG_SHA256="b5291bc4e6d69ebbd3da3800c5bf4a70f19bb92679b23e09b3b612c1e648d1ff"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://www.wavpack.com"
PKG_URL="https://www.wavpack.com/wavpack-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libiconv"
PKG_LONGDESC="Audio compression format providing lossless, high-quality lossy and hybrid compression mode."

PKG_CMAKE_OPTS_TARGET="-DBUILD_TESTING=OFF \
                       -DCMAKE_INSTALL_INCLUDEDIR=include/wavpack \
                       -DWAVPACK_BUILD_PROGRAMS=OFF \
                       -DWAVPACK_ENABLE_ASM=OFF \
                       -DWAVPACK_INSTALL_DOCS=OFF"
