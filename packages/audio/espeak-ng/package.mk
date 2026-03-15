# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="espeak-ng"
PKG_VERSION="0d451f8c1c6ae837418b823bd9c4cbc574ea9ff5"
PKG_SHA256="e0c9737c57f07bf351d3ae85d8c1e90faae36302d697ba2d646780e2b2e8f41a"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/espeak-ng/espeak-ng"
PKG_URL="https://github.com/espeak-ng/espeak-ng/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="cmake:host ninja:host sonic:host"
PKG_DEPENDS_TARGET="toolchain espeak-ng:host sonic"
PKG_LONGDESC="eSpeak NG is an open source speech synthesizer that supports more than a hundred languages and accents"
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_HOST="-DBUILD_SHARED_LIBS=OFF \
                     -DCOMPILE_INTONATIONS=OFF \
                     -DENABLE_TESTS=OFF \
                     -DUSE_LIBSONIC=ON"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=ON \
                       -DCOMPILE_INTONATIONS=ON \
                       -DENABLE_TESTS=OFF \
                       -DUSE_LIBSONIC=ON \
                       -DNativeBuild_DIR=${TOOLCHAIN}/bin"

pre_configure_target() {
  unset VALGRIND
}

post_makeinstall_target() {
  safe_remove ${INSTALL}/usr/share/vim

  # add symlink for backwards compatibility with old programs
  ln -sf espeak-ng ${INSTALL}/usr/bin/espeak
}
