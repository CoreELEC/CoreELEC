# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXfont2"
PKG_VERSION="2.0.8"
PKG_SHA256="f556c0e1093a4e6911cc90bc4b106d201902ee187fd74af206ff162f7e6a24d5"
PKG_LICENSE="MIT AND BSD-4-Clause AND BSD-2-Clause AND HPND-sell-variant"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros xtrans freetype libfontenc"
PKG_LONGDESC="X font Library"

PKG_CONFIGURE_OPTS_TARGET="--disable-ipv6 \
                           --enable-freetype \
                           --enable-builtins \
                           --disable-pcfformat \
                           --disable-bdfformat \
                           --disable-snfformat \
                           --enable-fc \
                           --with-gnu-ld \
                           --without-xmlto"

post_configure_target() {
  libtool_remove_rpath libtool
}
