# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2024-present Team CoreELEC (https://coreelec.org)

. $(get_pkg_directory glib)/package.mk

# this package is build only for target
PKG_NAME="glib-initial"
PKG_URL=""
# remove circular dependency
PKG_DEPENDS_TARGET="${PKG_DEPENDS_TARGET//gobject-introspection/}"

unpack() {
  mkdir -p ${PKG_BUILD}
  tar --strip-components=1 -xf ${SOURCES}/glib/glib-${PKG_VERSION}.tar.xz -C ${PKG_BUILD}
}

# this also overwrites function from main package
pre_configure_target() {
  PKG_MESON_OPTS_TARGET="${PKG_MESON_OPTS_TARGET/introspection=enabled/introspection=disabled}"
  PKG_MESON_OPTS_TARGET+=" -Dsysprof=disabled"
}
