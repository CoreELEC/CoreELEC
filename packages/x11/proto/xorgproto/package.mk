# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xorgproto"
PKG_VERSION="2026.1"
PKG_SHA256="f9bfe4a9ed8c8ab9d2a3b0d49797f046052dadd06b7a8b45dbffaffb137e8290"
PKG_LICENSE="MIT"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/proto/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros"
PKG_LONGDESC="combined X.Org X11 Protocol headers"

PKG_MESON_OPTS_TARGET="-Dlegacy=false"
