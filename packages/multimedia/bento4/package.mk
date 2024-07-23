# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bento4"
PKG_VERSION="1.6.0-641-2-Omega"
PKG_SHA256="3e4970a33700f1844513bb0c86c0c2de9f638bdc03547d2b7421f558962b85f1"
PKG_LICENSE="GPL"
PKG_SITE="https://www.bento4.com"
PKG_URL="https://github.com/xbmc/Bento4/archive/refs/tags/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_DEPENDS_UNPACK="inputstream.adaptive"
PKG_LONGDESC="C++ class library and tools designed to read and write ISO-MP4 files"
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DBUILD_APPS=OFF"
