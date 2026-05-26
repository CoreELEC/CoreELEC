# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXext"
PKG_VERSION="1.3.7"
PKG_SHA256="6c643c7035cdacf67afd68f25d01b90ef889d546c9fcd7c0adf7c2cf91e3a32d"
PKG_LICENSE="MIT AND HPND-sell-variant"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libX11"
PKG_LONGDESC="LibXext provides an X Window System client interface to several extensions to the X protocol."

PKG_CONFIGURE_OPTS_TARGET="--enable-malloc0returnsnull --without-xmlto"

post_configure_target() {
  libtool_remove_rpath libtool
}
