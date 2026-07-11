# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libffi"
PKG_VERSION="3.7.0"
PKG_SHA256="2255c5a638dfb51bf67c20a12a7bb70d17feb1e9eababac05f5573146f586436"
PKG_LICENSE="MIT"
PKG_SITE="http://sourceware.org/${PKG_NAME}/"
PKG_URL="https://github.com/libffi/libffi/releases/download/v${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="ccache:host autoconf:host automake:host libtool:host pkg-config:host"
PKG_DEPENDS_TARGET="autotools:host gcc:host"
PKG_LONGDESC="Foreign Function Interface Library."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--disable-debug \
                           --enable-static --disable-shared \
                           --with-pic \
                           --enable-structs \
                           --enable-raw-api \
                           --disable-purify-safety \
                           --with-gnu-ld"
