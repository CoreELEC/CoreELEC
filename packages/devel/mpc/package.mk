# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mpc"
PKG_VERSION="1.4.1"
PKG_SHA256="91204cd32f164bd3b7c992d4a6a8ce6519511aadab30f78b6982d0bf8d73e931"
PKG_LICENSE="LGPL"
PKG_SITE="https://www.multiprecision.org"
PKG_URL="https://ftpmirror.gnu.org/mpc/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="ccache:host gmp:host mpfr:host"
PKG_LONGDESC="A C library for the arithmetic of complex numbers with arbitrarily high precision and correct rounding of the result."

PKG_CONFIGURE_OPTS_HOST="--target=${TARGET_NAME} \
                         --enable-static --disable-shared \
                         --with-gmp=${TOOLCHAIN} \
                         --with-mpfr=${TOOLCHAIN}"
