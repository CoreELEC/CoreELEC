# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libogg"
PKG_VERSION="1.3.6"
PKG_SHA256="5c8253428e181840cd20d41f3ca16557a9cc04bad4a3d04cce84808677fa1061"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://www.xiph.org/ogg/"
PKG_URL="https://downloads.xiph.org/releases/ogg/libogg-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Libogg contains necessary functionality to create, decode, and work with Ogg bitstreams."
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DINSTALL_DOCS=OFF"
