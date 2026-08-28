# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cmake"
PKG_VERSION="4.4.3"
PKG_SHA256="c46400618b4f1f2b43507f24fb22f3ae830c3416cf23b776e16e1d413aa892f0"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://cmake.org/"
PKG_URL="https://cmake.org/files/v$(get_pkg_version_maj_min)/cmake-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="pkg-config:host"
PKG_LONGDESC="A cross-platform, open-source make system."
PKG_TOOLCHAIN="configure"
PKG_BUILD_FLAGS="+local-cc"

configure_host() {
  ../configure --prefix=${TOOLCHAIN} \
               --no-qt-gui --no-system-libs \
               -- \
               -DCMAKE_C_FLAGS="-O2 -Wall -pipe -Wno-format-security" \
               -DCMAKE_CXX_FLAGS="-O2 -Wall -pipe -Wno-format-security" \
               -DCMAKE_EXE_LINKER_FLAGS="${HOST_LDFLAGS}" \
               -DCMAKE_USE_OPENSSL=OFF \
               -DBUILD_CursesDialog=0
}
