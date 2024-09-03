# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="json-glib"
PKG_VERSION="1.10.0"
PKG_SHA256="447890f9de2a04c312871768208f6c8aeec4069392af7605bc77e61165dcb374"
PKG_LICENSE="LGPL-2.1"
PKG_SITE="https://github.com/GNOME/json-glib"
PKG_URL="https://github.com/GNOME/json-glib/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib"
PKG_LONGDESC="JSON-GLib implements a full suite of JSON-related tools using GLib and GObject."

PKG_MESON_OPTS_TARGET="-Dintrospection=disabled \
                       -Dgtk_doc=disabled \
                       -Dman=false \
                       -Dtests=false"
