# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pango"
PKG_VERSION="1.54.0"
PKG_SHA256="8a9eed75021ee734d7fc0fdf3a65c3bba51dfefe4ae51a9b414a60c70b2d1ed8"
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
