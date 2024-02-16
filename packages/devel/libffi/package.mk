# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libffi"
PKG_VERSION="3.4.5"
PKG_SHA256="96fff4e589e3b239d888d9aa44b3ff30693c2ba1617f953925a70ddebcc102b2"
PKG_LICENSE="GPL"
PKG_SITE="http://sourceware.org/${PKG_NAME}/"
PKG_URL="https://github.com/libffi/libffi/releases/download/v${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="ccache:host autoconf:host automake:host libtool:host pkg-config:host"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Foreign Function Interface Library."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--disable-debug \
                           --enable-static --disable-shared \
                           --with-pic \
                           --enable-structs \
                           --enable-raw-api \
                           --disable-purify-safety \
                           --with-gnu-ld"
