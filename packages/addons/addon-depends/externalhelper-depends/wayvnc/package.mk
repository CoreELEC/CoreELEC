# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wayvnc"
PKG_VERSION="0.10.1"
PKG_SHA256="1dcb54f58d1637995bfb59c17709efca7833bae41f31b33eb47e608668a89d66"
PKG_LICENSE="ISC"
PKG_SITE="https://github.com/any1/wayvnc"
PKG_URL="https://github.com/any1/wayvnc/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain aml jansson libdrm mesa neatvnc wayland wayland-protocols"
PKG_SHORTDESC="A VNC server for wlroots based Wayland compositors"
PKG_BUILD_FLAGS="-sysroot"

PKG_DEPENDS_CONFIG="wayland wayland-protocols"

PKG_MESON_OPTS_TARGET="-Dman-pages=disabled \
                       -Dpam=disabled \
                       -Dscreencopy-dmabuf=enabled \
                       -Dsystemtap=false \
                       -Dtests=false"
