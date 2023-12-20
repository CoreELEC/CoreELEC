# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="x264"
PKG_VERSION="a354f11f8f019a2a34ae7ef554ff07b31f0818f3"
PKG_SHA256="3f87b564f71e7fccf2b58c1bafcdb74b88ce2d57fff5cf870a5703fb33691d38"
PKG_LICENSE="GPL"
PKG_SITE="http://www.videolan.org/developers/x264.html"
PKG_URL="https://code.videolan.org/videolan/x264/-/archive/${PKG_VERSION}/x264-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="x264 codec"

if [ "${TARGET_ARCH}" = "x86_64" ]; then
  PKG_DEPENDS_TARGET+=" nasm:host"
fi

pre_configure_target() {
  cd ${PKG_BUILD}
  rm -rf .${TARGET_NAME}

  if [ "${TARGET_ARCH}" = "x86_64" ]; then
    export AS="${TOOLCHAIN}/bin/nasm"
  else
    PKG_X264_ASM="--disable-asm"
  fi
}

configure_target() {
  ./configure \
    --cross-prefix="${TARGET_PREFIX}" \
    --extra-cflags="${CFLAGS}" \
    --extra-ldflags="${LDFLAGS}" \
    --host="${TARGET_NAME}" \
    --prefix="/usr" \
    --sysroot="${SYSROOT_PREFIX}" \
    ${PKG_X264_ASM} \
    --disable-cli \
    --enable-lto \
    --enable-pic \
    --enable-static \
    --enable-strip
}
