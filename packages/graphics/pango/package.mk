# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pango"
PKG_VERSION="1.51.2"
PKG_SHA256="3dba407f2b5fc117e192f3025f0a1cc8edc1fd9b934b1c578b2b97342139415a"
PKG_LICENSE="GPL"
PKG_SITE="http://www.pango.org/"
PKG_URL="https://download.gnome.org/sources/pango/${PKG_VERSION:0:4}/pango-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain cairo freetype fontconfig fribidi glib json-glib harfbuzz"
PKG_DEPENDS_CONFIG="cairo"
PKG_LONGDESC="The Pango library for layout and rendering of internationalized text."

configure_package() {
  # Build with X11 support
  if [ ${DISPLAYSERVER} = "x11" ]; then
    PKG_DEPENDS_TARGET+=" libX11 libXft"
    PKG_DEPENDS_CONFIG+=" libXft"
    PKG_BUILD_FLAGS="-sysroot"
  fi
}

pre_configure_target() {
  PKG_MESON_OPTS_TARGET="-Dgtk_doc=false \
                         -Dintrospection=disabled"
}
