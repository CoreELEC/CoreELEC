# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cage"
PKG_VERSION="0.3.0"
PKG_SHA256="f32e6885444e365de3bc076d307c20eff59ee42ed0237219eebd3d2fe597e289"
PKG_LICENSE="MIT"
PKG_SITE="https://www.hjdskes.nl/projects/cage"
PKG_URL="https://github.com/cage-kiosk/cage/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libxkbcommon seatd wayland wayland-protocols wlroots"
PKG_DEPENDS_CONFIG="seatd wayland wayland-protocols wlroots"
PKG_LONGDESC="A Wayland kiosk "
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dman-pages=disabled"
