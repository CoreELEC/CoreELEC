# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cage"
PKG_VERSION="0.3.1"
PKG_SHA256="6dc1619665acd367e0174c93b234002549a66f55f1de9197d67f0305415babc8"
PKG_LICENSE="MIT"
PKG_SITE="https://www.hjdskes.nl/projects/cage"
PKG_URL="https://github.com/cage-kiosk/cage/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libxkbcommon seatd wayland wayland-protocols wlroots"
PKG_DEPENDS_CONFIG="seatd wayland wayland-protocols wlroots"
PKG_LONGDESC="A Wayland kiosk "
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dman-pages=disabled"
