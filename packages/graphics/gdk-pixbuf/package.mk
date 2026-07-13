# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="gdk-pixbuf"
PKG_VERSION="2.44.7"
PKG_SHA256="172f80e3626ec31520a970400f1a3694e04718f6c2cd2885f75250fb5a6995a4"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="http://www.gtk.org/"
PKG_URL="https://ftp.gnome.org/pub/gnome/sources/gdk-pixbuf/${PKG_VERSION:0:4}/gdk-pixbuf-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain glib libjpeg-turbo libpng jasper shared-mime-info tiff"
PKG_DEPENDS_CONFIG="shared-mime-info"
PKG_LONGDESC="GdkPixbuf is a a GNOME library for image loading and manipulation."

configure_package() {
  if [ "${DISPLAYSERVER}" = "x11" ]; then
    PKG_DEPENDS_TARGET+=" libX11"
  fi
}

pre_configure_target() {
  PKG_MESON_OPTS_TARGET="-Ddocumentation=false \
                         -Dintrospection=disabled \
                         -Dman=false \
                         -Drelocatable=false \
                         -Dinstalled_tests=false \
                         -Dglycin=disabled \
                         -Dtests=false"

  if [ "${DISPLAYSERVER}" != "x11" ]; then
    PKG_MESON_OPTS_TARGET+=" -Dbuiltin_loaders=all"
  fi
}
