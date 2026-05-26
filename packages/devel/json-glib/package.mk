# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="json-glib"
PKG_VERSION="1.10.8"
PKG_SHA256="7a114bdac0b2611a7207e981c37fa9b1e70d9cb642470cd9e967b135428cec52"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://github.com/GNOME/json-glib"
PKG_URL="https://github.com/GNOME/json-glib/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib"
PKG_LONGDESC="JSON-GLib implements a full suite of JSON-related tools using GLib and GObject."

PKG_MESON_OPTS_TARGET="-Dintrospection=disabled \
                       -Ddocumentation=disabled \
                       -Dman=false \
                       -Dtests=false"
