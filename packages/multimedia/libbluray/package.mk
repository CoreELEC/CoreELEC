# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libbluray"
PKG_VERSION="1.0.2"
PKG_SHA256="6d9e7c4e416f664c330d9fa5a05ad79a3fb39b95adfc3fd6910cbed503b7aeff"
PKG_LICENSE="LGPL"
PKG_SITE="https://www.videolan.org/developers/libbluray.html"
PKG_URL="http://download.videolan.org/pub/videolan/libbluray/$PKG_VERSION/$PKG_NAME-$PKG_VERSION.tar.bz2"
PKG_DEPENDS_TARGET="toolchain fontconfig freetype libxml2"
PKG_LONGDESC="libbluray is an open-source library designed for Blu-Ray Discs playback for media players."
PKG_TOOLCHAIN="autotools"

if [ "$BLURAY_AACS_SUPPORT" = "yes" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET libaacs"
fi

if [ "$BLURAY_BDPLUS_SUPPORT" = "yes" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET libbdplus"
fi

PKG_CONFIGURE_OPTS_TARGET="--disable-werror \
                           --disable-extra-warnings \
                           --disable-optimizations \
                           --disable-examples \
                           --disable-bdjava-jar \
                           --disable-doxygen-doc \
                           --disable-doxygen-dot \
                           --disable-doxygen-man \
                           --disable-doxygen-rtf \
                           --disable-doxygen-xml \
                           --disable-doxygen-chm \
                           --disable-doxygen-chi \
                           --disable-doxygen-html \
                           --disable-doxygen-ps \
                           --disable-doxygen-pdf \
                           --with-freetype \
                           --with-fontconfig \
                           --with-libxml2 \
                           --with-gnu-ld"
