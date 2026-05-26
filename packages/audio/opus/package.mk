# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="opus"
PKG_VERSION="1.6.1"
PKG_SHA256="6ffcb593207be92584df15b32466ed64bbec99109f007c82205f0194572411a1"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="http://www.opus-codec.org"
PKG_URL="https://ftp.osuosl.org/pub/xiph/releases/opus/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Codec designed for interactive speech and audio transmission over the Internet."
PKG_TOOLCHAIN="configure"
PKG_BUILD_FLAGS="+pic"

if [ "${TARGET_ARCH}" = "arm" ]; then
  PKG_FIXED_POINT="--enable-fixed-point"
else
  PKG_FIXED_POINT="--disable-fixed-point"
fi

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           ${PKG_FIXED_POINT}"
