# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="espeak-ng"
PKG_VERSION="fbe4b3764285c35b1f035cb8d09ad9fc19f71c30"
PKG_SHA256="8a997899a38d55a5035b934539b497ff033a15976ce99377d708929a686ba4c3"
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
