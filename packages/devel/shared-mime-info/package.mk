# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="shared-mime-info"
PKG_VERSION="2.5.1"
PKG_SHA256="b75b420da9b0be9a3d99b1bee6ed87957b56ab54583ac1a97fbd0dc98ddddb25"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://freedesktop.org/wiki/Software/shared-mime-info/"
PKG_URL="https://gitlab.freedesktop.org/xdg/${PKG_NAME}/-/archive/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_HOST="toolchain:host glib:host libxml2:host gettext:host itstool:host"
PKG_DEPENDS_TARGET="toolchain glib libxml2 gettext shared-mime-info:host"
PKG_LONGDESC="The shared-mime-info package contains the core database of common types."
PKG_BUILD_FLAGS="-parallel"

configure_package() {
  # Sway Support
  if [ ! "${WINDOWMANAGER}" = "sway" ]; then
    PKG_BUILD_FLAGS+=" -sysroot"
  fi
}

PKG_MESON_OPTS_COMMON="-Dupdate-mimedb=false \
                       -Dbuild-spec=false \
                       -Dbuild-tests=false"

PKG_MESON_OPTS_HOST="${PKG_MESON_OPTS_COMMON}"
PKG_MESON_OPTS_TARGET="${PKG_MESON_OPTS_COMMON}"

post_makeinstall_target() {
  # Create /usr/share/mime/mime.cache
  if [ "${WINDOWMANAGER}" = "sway" ]; then
    ${TOOLCHAIN}/bin/update-mime-database ${INSTALL}/usr/share/mime
  fi
}
