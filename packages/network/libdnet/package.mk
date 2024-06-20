# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libdnet"
PKG_VERSION="1.18.0"
PKG_SHA256="a4a82275c7d83b85b1daac6ebac9461352731922161f1dcdcccd46c318f583c9"
PKG_LICENSE="BSD"
PKG_SITE="https://github.com/ofalk/libdnet"
PKG_URL="https://github.com/ofalk/libdnet/archive/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A simplified, portable interface to several low-level networking routines"
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+pic"

pre_configure_target() {
  PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                             --disable-shared \
                             --enable-check=no"

  export CFLAGS+=" -I${PKG_BUILD}/include"
}

post_makeinstall_target() {
  cp ${SYSROOT_PREFIX}/usr/bin/dnet-config \
     ${TOOLCHAIN}/bin/dnet-config
}
