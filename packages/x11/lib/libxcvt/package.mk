# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libxcvt"
PKG_VERSION="0.1.3"
PKG_SHA256="a929998a8767de7dfa36d6da4751cdbeef34ed630714f2f4a767b351f2442e01"
PKG_LICENSE="MIT AND HPND-sell-variant"
PKG_SITE="https://gitlab.freedesktop.org/xorg/lib/libxcvt"
PKG_URL="https://www.x.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libxcvt is a library providing a standalone version of the X server implementation of the VESA CVT standard timing modelines generator."
PKG_BUILD_FLAGS="+pic"
