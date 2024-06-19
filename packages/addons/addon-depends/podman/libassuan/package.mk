# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libassuan"
PKG_VERSION="3.0.1"
PKG_SHA256="c8f0f42e6103dea4b1a6a483cb556654e97302c7465308f58363778f95f194b1"
PKG_LICENSE="LGPLv2.1+"
PKG_SITE="https://gnupg.org/software/libassuan/index.html"
PKG_URL="https://www.gnupg.org/ftp/gcrypt/libassuan/libassuan-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain libgpg-error"
PKG_LONGDESC="Libassuan is a small library implementing the so-called Assuan protocol"
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --with-gnu-ld \
                           --with-pic \
                           --with-libgpg-error-prefix=${SYSROOT_PREFIX}/usr"
