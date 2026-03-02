# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="glib"
PKG_VERSION="2.87.3"
PKG_SHA256="8374eb3afcf9b6d21d5b5960b324922bb4960ffe438df5ceface9e4fafb2f0a4"
PKG_LICENSE="LGPL"
PKG_SITE="https://www.gtk.org/"
PKG_URL="https://download.gnome.org/sources/glib/$(get_pkg_version_maj_min)/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="libffi:host pcre2:host Python3:host meson:host ninja:host"
PKG_DEPENDS_TARGET="meson:host ninja:host gcc:host glib:host libffi pcre2 Python3:host util-linux zlib gobject-introspection"
PKG_LONGDESC="A library which includes support routines for C such as lists, trees, hashes, memory allocation."

PKG_MESON_OPTS_HOST="-Ddefault_library=shared \
                     -Dinstalled_tests=false \
                     -Dlibmount=disabled \
                     -Dintrospection=disabled \
                     -Dsysprof=disabled \
                     -Dtests=false"

PKG_MESON_OPTS_TARGET="-Ddefault_library=shared \
                       -Dinstalled_tests=false \
                       -Dselinux=disabled \
                       -Dxattr=true \
                       -Ddocumentation=false \
                       -Dman-pages=disabled \
                       -Ddtrace=disabled \
                       -Dsystemtap=disabled \
                       -Dbsymbolic_functions=true \
                       -Dsysprof=disabled \
                       -Dtests=false \
                       -Dintrospection=enabled"

pre_configure_target() {
  # tweak the binary names so that it picks up our
  # wrappers which do the cross-compile with qemu
  sed -e "s|gir_scanner = .*|gir_scanner = files('${TOOLCHAIN}/bin/g-ir-scanner-wrapper')|" \
      -e "s|enable_gir = .*|enable_gir = true|" \
      -e "s|  error('Running binaries|  # error('Running binaries|" \
      -i ${PKG_BUILD}/meson.build

  sed -e "s|override_find_program('g-ir-compiler'|override_find_program('g-ir-compiler-wrapper'|" \
      -i ${PKG_BUILD}/girepository/compiler/meson.build
}

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
  rm -rf ${INSTALL}/usr/lib/gdbus-2.0
  rm -rf ${INSTALL}/usr/lib/glib-2.0
  rm -rf ${INSTALL}/usr/lib/installed-tests
  rm -rf ${INSTALL}/usr/share

  # glib binaries must be executed from toolchain
  sed -e "s#bindir=\${prefix}/bin#bindir=${TOOLCHAIN}/bin#" -i "${SYSROOT_PREFIX}/usr/lib/pkgconfig/"{gio,glib}-2.0.pc
}
