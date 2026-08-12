# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xdg-dbus-proxy"
PKG_VERSION="0.1.8"
PKG_SHA256="722e2a327acd2cd053b864e65f2f507ba02f966d3622a50040f4e3486f50c9c4"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://github.com/flatpak/xdg-dbus-proxy"
PKG_URL="https://github.com/flatpak/xdg-dbus-proxy/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib"
PKG_LONGDESC="a filtering proxy for D-Bus connections"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dman=disabled \
                       -Dtests=false"
