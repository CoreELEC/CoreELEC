# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="lame"
PKG_VERSION="4.0"
PKG_SHA256="3df5124d5ad3a98312ffd7ba6a9b36230e4f8a3e66d3ce0f425e336c32d216eb"
PKG_LICENSE="LGPL-2.0-or-later"
PKG_SITE="http://lame.sourceforge.net/"
PKG_URL="https://downloads.sourceforge.net/project/lame/lame/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A high quality MPEG Audio Layer III (MP3) encoder."
PKG_BUILD_FLAGS="-parallel +pic -sysroot"

# package specific configure options
PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-nasm \
                           --disable-rpath \
                           --disable-cpml \
                           --disable-gtktest \
                           --disable-efence \
                           --disable-analyzer-hooks \
                           --disable-decoder \
                           --disable-frontend \
                           --disable-mp3x \
                           --disable-mp3rtp \
                           --disable-dynamic-frontends \
                           --enable-expopt=no \
                           --enable-debug=no \
                           --with-gnu-ld \
                           --with-fileio=lame \
                           GTK_CONFIG=no"
