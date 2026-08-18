# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libevdev"
PKG_VERSION="1.13.7"
PKG_SHA256="0caf824971108f15bb2ad356433bae198d7d3bf1e82d43f63626e069e060bfa6"
PKG_LICENSE="MIT"
PKG_SITE="http://www.freedesktop.org/wiki/Software/libevdev/"
PKG_URL="http://www.freedesktop.org/software/libevdev/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libevdev is a wrapper library for evdev devices."
PKG_BUILD_FLAGS="+pic"

PKG_MESON_OPTS_TARGET=" \
  -Ddefault_library=shared \
  -Ddocumentation=disabled \
  -Dtests=disabled"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
}
