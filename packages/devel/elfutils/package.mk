# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="elfutils"
PKG_VERSION="0.173"
PKG_SHA256="b76d8c133f68dad46250f5c223482c8299d454a69430d9aa5c19123345a000ff"
PKG_LICENSE="GPL"
PKG_SITE="https://sourceware.org/elfutils/"
PKG_URL="https://sourceware.org/elfutils/ftp/$PKG_VERSION/$PKG_NAME-$PKG_VERSION.tar.bz2"
PKG_DEPENDS_HOST="make:host zlib:host"
PKG_DEPENDS_TARGET="toolchain zlib"
PKG_LONGDESC="A collection of utilities to handle ELF objects."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="utrace_cv_cc_biarch=false \
                           --disable-nls \
                           --with-zlib \
                           --without-bzlib \
                           --without-lzma"

PKG_CONFIGURE_OPTS_HOST="utrace_cv_cc_biarch=false \
                           --disable-nls \
                           --with-zlib \
                           --without-bzlib \
                           --without-lzma"
