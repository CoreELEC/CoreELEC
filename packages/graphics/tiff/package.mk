# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tiff"
PKG_VERSION="4.7.2"
PKG_SHA256="672bd7d10aee4606171afb864f3570b83340f6a33e2c186dc0512f7145ffdf6a"
PKG_LICENSE="libtiff"
PKG_SITE="https://libtiff.gitlab.io/libtiff/"
PKG_URL="https://download.osgeo.org/libtiff/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libjpeg-turbo zlib"
PKG_LONGDESC="libtiff is a library for reading and writing TIFF files."
PKG_BUILD_FLAGS="+pic -gold"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -Dtiff-tools=OFF \
                       -Dtiff-tests=OFF \
                       -Dtiff-contrib=OFF \
                       -Dtiff-docs=OFF \
                       -Dmdi=OFF \
                       -Djbig=OFF \
                       -Dlzma=OFF \
                       -Dzstd=OFF \
                       -Dwebp=OFF \
                       -Dtiff-cxx=ON \
                       -Djpeg=ON"
