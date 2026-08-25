# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libheif"
PKG_VERSION="1.23.2"
PKG_SHA256="8bd5d41d19dc84536d118b04774709f244df6104ef66d623dad5fa4650143405"
PKG_LICENSE="LGPL-3.0-or-later"
PKG_SITE="https://www.libde265.org"
PKG_URL="https://github.com/strukturag/libheif/releases/download/v${PKG_VERSION}/libheif-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libde265 libjpeg-turbo libpng"
PKG_LONGDESC="A HEIF file format decoder and encoder."
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -DWITH_EXAMPLES=OFF \
                       -DWITH_AOM_DECODER=OFF \
                       -DWITH_AOM_ENCODER=OFF"

pre_configure_target() {
  export CXXFLAGS="${CXXFLAGS} -Wno-unused-variable"
}
