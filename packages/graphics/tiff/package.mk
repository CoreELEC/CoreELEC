# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tiff"
PKG_VERSION="4.7.1"
PKG_SHA256="f698d94f3103da8ca7438d84e0344e453fe0ba3b7486e04c5bf7a9a3fabe9b69"
PKG_LICENSE="LicenseRef-libtiff"
PKG_SITE="http://www.remotesensing.org/libtiff/"
PKG_URL="http://download.osgeo.org/libtiff/${PKG_NAME}-${PKG_VERSION}.tar.gz"
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
