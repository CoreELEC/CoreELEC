# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXi"
PKG_VERSION="1.8.2"
PKG_SHA256="d0e0555e53d6e2114eabfa44226ba162d2708501a25e18d99cfb35c094c6c104"
PKG_LICENSE="OSS"
PKG_SITE="https://www.x.org/"
PKG_URL="https://www.x.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libX11 libXfixes libXext"
PKG_LONGDESC="LibXi provides an X Window System client interface to the XINPUT extension to the X protocol."
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--enable-malloc0returnsnull \
                           --disable-silent-rules \
                           --disable-docs \
                           --disable-specs \
                           --without-xmlto \
                           --without-fop \
                           --without-xsltproc \
                           --without-asciidoc \
                           --with-gnu-ld"

post_configure_target() {
  libtool_remove_rpath libtool
}
