# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wayland-protocols"
PKG_VERSION="1.48"
PKG_SHA256="398036ac0eb6484982ddbde7ff86848d753231f9cdeeae983f06b52946625aa1"
PKG_LICENSE="OSS"
PKG_SITE="https://wayland.freedesktop.org/"
PKG_URL="https://gitlab.freedesktop.org/wayland/${PKG_NAME}/-/releases/${PKG_VERSION}/downloads/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain wayland:host"
PKG_LONGDESC="Specifications of extended Wayland protocols"

if [ "${DISPLAYSERVER}" != "wl" ]; then
  PKG_BUILD_FLAGS="-sysroot"
fi

PKG_MESON_OPTS_TARGET="-Dtests=false"

post_makeinstall_target() {
  if [ "${DISPLAYSERVER}" = "wl" ]; then
    safe_remove ${INSTALL}
  else
    sed -e "s|^pkgdatadir=.*\$|pkgdatadir=${INSTALL}/usr/share/wayland-protocols|" \
        -i "${INSTALL}/usr/share/pkgconfig/wayland-protocols.pc"
  fi
}
