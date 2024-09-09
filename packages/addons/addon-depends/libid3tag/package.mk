# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libid3tag"
PKG_VERSION="0.16.3"
PKG_SHA256="0561009778513a95d91dac33cee8418d6622f710450a7cb56a74636d53b588cb"
PKG_LICENSE="GPL"
PKG_SITE="https://www.underbit.com/products/mad/"
PKG_URL="https://codeberg.org/tenacityteam/libid3tag/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain zlib"
PKG_LONGDESC="A library for id3 tagging."

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF"
