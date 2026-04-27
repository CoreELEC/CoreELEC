# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="espeak-ng"
PKG_VERSION="724808c5a83f9ef95fdd0db886ba7ba537ff224a"
PKG_SHA256="7d8ebd78201923a443245362a928999977d1721c3eeba596eafc6ea8db7e243c"
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
                     -DESPEAK_COMPAT=OFF \
                     -DUSE_LIBSONIC=ON"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=ON \
                       -DCOMPILE_INTONATIONS=ON \
                       -DENABLE_TESTS=OFF \
                       -DESPEAK_COMPAT=OFF \
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
