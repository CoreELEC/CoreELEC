# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xdg-dbus-proxy"
PKG_VERSION="0.1.9"
PKG_SHA256="88e793b5f89a4ff55c7212d8f9cda38fcf8434614bd05669d4c6562622d63bdd"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://github.com/flatpak/xdg-dbus-proxy"
PKG_URL="https://github.com/flatpak/xdg-dbus-proxy/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib"
PKG_LONGDESC="a filtering proxy for D-Bus connections"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dman=disabled \
                       -Dtests=false"
