# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="st"
PKG_VERSION="0.9.3"
PKG_SHA256="9ed9feabcded713d4ded38c8cebf36a3b08f0042ef7934a0e2b2409da56e649b"
PKG_ARCH="x86_64"
PKG_LICENSE="MIT"
PKG_SITE="https://st.suckless.org/"
PKG_URL="https://dl.suckless.org/st/st-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libX11 libXft libXrender fontconfig freetype ncurses"
PKG_LONGDESC="A simple terminal implementation for X"
PKG_BUILD_FLAGS="-sysroot"

PKG_MAKE_OPTS_TARGET="X11INC=$(get_install_dir libXft)/usr/include \
                      X11LIB=$(get_install_dir libXft)/usr/lib"

pre_configure_target() {
  LDFLAGS="-lXrender ${LDFLAGS}"
}
