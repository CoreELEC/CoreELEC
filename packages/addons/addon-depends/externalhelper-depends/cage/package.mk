# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cage"
PKG_VERSION="0.2.1"
PKG_SHA256="acab0c83175164a788d7b9f89338cbdebdc4f7197aff6fdc267c32f7181234a9"
PKG_LICENSE="MIT"
PKG_SITE="https://www.hjdskes.nl/projects/cage"
PKG_URL="https://github.com/cage-kiosk/cage/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libxkbcommon seatd wayland wayland-protocols wlroots"
PKG_DEPENDS_CONFIG="seatd wayland wayland-protocols wlroots"
PKG_LONGDESC="A Wayland kiosk "
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dman-pages=disabled"
