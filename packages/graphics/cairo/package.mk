# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cairo"
PKG_VERSION="1.18.6"
PKG_SHA256="1c767308174337a74694da0f3ec069c271452163a1ef4540964c50c301f157d4"
PKG_LICENSE="LGPL-2.1-or-later OR MPL-1.1"
PKG_SITE="https://cairographics.org/"
PKG_URL="https://cairographics.org/releases/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain zlib freetype fontconfig glib libpng pixman"
PKG_LONGDESC="Cairo is a vector graphics library with cross-device output support."

configure_package() {
  if [ "${DISPLAYSERVER}" = "x11" ]; then
    PKG_DEPENDS_TARGET+=" libXrender libX11 mesa"
  fi
}

pre_configure_target() {
  PKG_MESON_OPTS_TARGET="-Ddwrite=disabled \
                         -Dfontconfig=enabled \
                         -Dfreetype=enabled \
                         -Dpng=enabled \
                         -Dquartz=disabled \
                         -Dtee=disabled \
                         -Dxcb=disabled \
                         -Dxlib-xcb=disabled \
                         -Dzlib=enabled \
                         -Dtests=disabled \
                         -Dgtk2-utils=disabled \
                         -Dglib=enabled \
                         -Dspectre=disabled \
                         -Dsymbol-lookup=disabled \
                         -Dgtk_doc=false"

  if [ "${DISPLAYSERVER}" = "x11" ]; then
    PKG_MESON_OPTS_TARGET+=" -Dxlib=enabled"
  else
    PKG_MESON_OPTS_TARGET+=" -Dxlib=disabled"
  fi
}
