# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xdg-dbus-proxy"
PKG_VERSION="0.1.7"
PKG_SHA256="232d2eb9b456fa1e307322da1bfeb4849eb410750a2a16a5de51ed2c2eb919e6"
PKG_LICENSE="LGPL-2.1"
PKG_SITE="https://github.com/flatpak/xdg-dbus-proxy"
PKG_URL="https://github.com/flatpak/xdg-dbus-proxy/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib"
PKG_LONGDESC="a filtering proxy for D-Bus connections"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dman=disabled \
                       -Dtests=false"

