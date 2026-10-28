# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libraw"
PKG_VERSION="0.22.3"
PKG_SHA256="789f3c33466210a1a0907f747a97c715d21bbf4af712f257fcbbd90111b227e4"
PKG_LICENSE="LGPL-2.1-only OR CDDL-1.0"
PKG_SITE="https://www.libraw.org/"
PKG_URL="https://www.libraw.org/data/LibRaw-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libjpeg-turbo lcms2"
PKG_LONGDESC="A library for reading RAW files obtained from digital photo cameras (CRW/CR2, NEF, RAF, DNG, and others)"
PKG_BUILD_FLAGS="+pic"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-openmp \
                           --enable-jpeg \
                           --disable-jasper \
                           --enable-lcms \
                           --disable-examples"
