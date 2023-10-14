# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="pygobject"
PKG_VERSION="3.46.0"
PKG_SHA256="426008b2dad548c9af1c7b03b59df0440fde5c33f38fb5406b103a43d653cafc"
PKG_ARCH="any"
PKG_LICENSE="LGPL"
PKG_SITE="http://www.pygtk.org/"
PKG_URL="http://ftp.gnome.org/pub/GNOME/sources/pygobject/$(get_pkg_version_maj_min)/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain Python3 glib libffi gobject-introspection pgi"
PKG_LONGDESC="A convenient wrapper for the GObject+ library for use in Python programs."
PKG_TOOLCHAIN="meson"

pre_configure_target() {
  PKG_CONFIG_PATH="${SYSROOT_PREFIX}/usr/lib/pkgconfig"

  PKG_MESON_OPTS_TARGET=" \
    -Dpython=${TOOLCHAIN}/bin/${PKG_PYTHON_VERSION} \
    -Dpycairo=disabled \
    -Dtests=false"
}

post_makeinstall_target() {
  python_remove_source

  rm -rf ${INSTALL}/usr/bin
  rm -rf ${INSTALL}/usr/share/pygobject
}
