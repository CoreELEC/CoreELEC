# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXfont2"
PKG_VERSION="2.0.7"
PKG_SHA256="8b7b82fdeba48769b69433e8e3fbb984a5f6bf368b0d5f47abeec49de3e58efb"
PKG_LICENSE="OSS"
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
