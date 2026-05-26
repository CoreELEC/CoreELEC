# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xtrans"
PKG_VERSION="1.6.0"
PKG_SHA256="faafea166bf2451a173d9d593352940ec6404145c5d1da5c213423ce4d359e92"
PKG_LICENSE="MIT"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros"
PKG_LONGDESC="Abstract network code for X."
PKG_BUILD_FLAGS="-cfg-libs"

PKG_CONFIGURE_OPTS_TARGET="--without-xmlto"

post_makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/pkgconfig
    cp xtrans.pc ${SYSROOT_PREFIX}/usr/lib/pkgconfig
}
