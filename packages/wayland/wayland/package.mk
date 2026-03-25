# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wayland"
PKG_VERSION="1.25.0"
PKG_SHA256="c065f040afdff3177680600f249727e41a1afc22fccf27222f15f5306faa1f03"
PKG_LICENSE="OSS"
PKG_SITE="https://wayland.freedesktop.org/"
PKG_URL="https://gitlab.freedesktop.org/wayland/wayland/-/releases/${PKG_VERSION}/downloads/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="libffi:host expat:host libxml2:host meson:host"
PKG_DEPENDS_TARGET="toolchain wayland:host libffi expat libxml2"
PKG_LONGDESC="a display server protocol"

PKG_BUILD_FLAGS="-ndebug"

PKG_MESON_OPTS_HOST="-Dlibraries=false \
                     -Dscanner=true \
                     -Dtests=false \
                     -Ddocumentation=false \
                     -Ddtd_validation=false"

PKG_MESON_OPTS_TARGET="-Dlibraries=true \
                       -Dscanner=false \
                       -Dtests=false \
                       -Ddocumentation=false \
                       -Ddtd_validation=false"

post_makeinstall_host() {
  cp ${TOOLCHAIN}/lib/pkgconfig/wayland-scanner.pc ${SYSROOT_PREFIX}/usr/lib/pkgconfig/
  mkdir -p ${SYSROOT_PREFIX}/usr/share/wayland
    cp ${TOOLCHAIN}/share/wayland/wayland.xml ${SYSROOT_PREFIX}/usr/share/wayland/
}
