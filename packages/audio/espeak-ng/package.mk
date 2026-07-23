# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="espeak-ng"
PKG_VERSION="2152f9c42916612f8cf0fc721aec9905a3c73420"
PKG_SHA256="798d8d6af592e03dad5da633a1c2b34fb31a05eb973d014c80721fcfa173e169"
PKG_LICENSE="GPL-3.0-or-later"
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
