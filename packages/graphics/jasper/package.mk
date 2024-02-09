# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="jasper"
PKG_VERSION="4.2.0"
PKG_SHA256="c9a3e35c95447f530b006fab6634a2dadec70276cc3e42c343b9e5ce5a1f2b6b"
PKG_LICENSE="OpenSource"
PKG_SITE="http://www.ece.uvic.ca/~mdadams/jasper/"
PKG_URL="https://github.com/jasper-software/jasper/archive/refs/tags/version-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libjpeg-turbo"
PKG_LONGDESC="A implementation of the ISO/IEC 15444-1 also known as JPEG-2000 standard for image compression."
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DJAS_ENABLE_DOC=false \
                       -DJAS_ENABLE_PROGRAMS=false \
                       -DJAS_ENABLE_SHARED=false \
                       -DALLOW_IN_SOURCE_BUILD=ON \
                       -DJAS_STDC_VERSION=201710L"

pre_configure_target() {
  export CFLAGS="${CFLAGS} -std=gnu17"
}
