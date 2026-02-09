# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="sbc"
PKG_VERSION="2.2"
PKG_SHA256="a1ada76ef35e5af9c2fbd063754dc9e37a8d989417c6eb1ecebb089b1383ae9e"
PKG_LICENSE="GPL"
PKG_SITE="http://www.bluez.org/"
PKG_URL="https://www.kernel.org/pub/linux/bluetooth/sbc-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="standalone SBC library"
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-tools \
                           --disable-tester"

post_makeinstall_target() {
  # fix static library
  sed -i 's/-lsbc/-lsbc -lbluetooth/' ${SYSROOT_PREFIX}/usr/lib/pkgconfig/sbc.pc
}
