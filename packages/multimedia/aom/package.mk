# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aom"
PKG_VERSION="3.13.3"
PKG_SHA256="446a4ae9741cb8f3eeb98c949d25f91b48cb2b8569cae975c4b737392e9024fc"
PKG_LICENSE="BSD"
PKG_SITE="https://www.webmproject.org"
PKG_URL="https://storage.googleapis.com/aom-releases/libaom-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="AV1 Codec Library"
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DENABLE_CCACHE=1 \
                       -DENABLE_DOCS=0 \
                       -DENABLE_EXAMPLES=0 \
                       -DENABLE_TESTS=0 \
                       -DENABLE_TOOLS=0"

#workaround gcc-14 erroring with neon declarations
if [ "${ARCH}" = "arm" ]; then
  TARGET_CFLAGS+=" -Wno-implicit-function-declaration"
fi

if [ "${TARGET_ARCH}" = "x86_64" ]; then
  PKG_DEPENDS_TARGET+=" nasm:host"
elif ! target_has_feature neon; then
  PKG_CMAKE_OPTS_TARGET+=" -DENABLE_NEON=0 -DENABLE_NEON_ASM=0"
fi
