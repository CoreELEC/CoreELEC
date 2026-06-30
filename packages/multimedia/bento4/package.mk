# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bento4"
PKG_VERSION="1.6.0-641-6-Piers"
PKG_SHA256="cb5006cd30889238760556e6148f69be34fef4dc48e8d895a9822f805504a5c0"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://www.bento4.com"
PKG_URL="https://github.com/xbmc/Bento4/archive/refs/tags/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_DEPENDS_UNPACK="inputstream.adaptive"
PKG_LONGDESC="C++ class library and tools designed to read and write ISO-MP4 files"
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DBUILD_APPS=OFF"
