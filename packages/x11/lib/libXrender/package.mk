# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXrender"
PKG_VERSION="0.9.12"
PKG_SHA256="b832128da48b39c8d608224481743403ad1691bf4e554e4be9c174df171d1b97"
PKG_LICENSE="HPND-sell-variant"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libX11"
PKG_LONGDESC="The X Rendering Extension introduces digital image composition within the X Window System."
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--enable-malloc0returnsnull"

post_configure_target() {
  libtool_remove_rpath libtool
}
