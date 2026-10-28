# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="poppler"
PKG_VERSION="26.10.0"
PKG_SHA256="6792cb7c69205007ad87d2e936cecc5b3a31fac29ab54ffc3175fdb6b2a6ce35"
PKG_LICENSE="GPL-2.0-only OR GPL-3.0-only"
PKG_SITE="https://poppler.freedesktop.org"
PKG_URL="https://poppler.freedesktop.org/poppler-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain zlib freetype fontconfig libjpeg-turbo libpng openjpeg"
PKG_LONGDESC="A PDF rendering library, used by vfs.pdf to display game manuals."
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -DBUILD_GTK_TESTS=OFF \
                       -DBUILD_QT5_TESTS=OFF \
                       -DBUILD_QT6_TESTS=OFF \
                       -DBUILD_CPP_TESTS=OFF \
                       -DBUILD_MANUAL_TESTS=OFF \
                       -DENABLE_CPP=ON \
                       -DENABLE_UTILS=OFF \
                       -DENABLE_GLIB=OFF \
                       -DENABLE_QT5=OFF \
                       -DENABLE_QT6=OFF \
                       -DENABLE_NSS3=OFF \
                       -DENABLE_GPGME=OFF \
                       -DENABLE_LIBCURL=OFF \
                       -DENABLE_BROTLI=OFF \
                       -DENABLE_HARFBUZZ=OFF \
                       -DENABLE_BOOST=OFF \
                       -DENABLE_LIBOPENJPEG=openjpeg2 \
                       -DENABLE_LIBTIFF=OFF \
                       -DENABLE_LCMS=OFF \
                       -DWITH_Cairo=OFF \
                       -DWITH_NSS3=OFF \
                       -DRUN_GPERF_IF_PRESENT=OFF"
