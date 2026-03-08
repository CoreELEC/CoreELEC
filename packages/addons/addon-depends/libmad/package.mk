# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2014 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libmad"
PKG_VERSION="60b2d91102fe94a9aaefe2b8b47efc3543bd8773"
PKG_SHA256="3b61791e63d2e71adabfae184c4b693cd6b43cc2a09f841edabc4f4c689f9518"
PKG_LICENSE="GPL"
PKG_SITE="http://www.mars.org/home/rob/proj/mpeg/"
PKG_URL="https://codeberg.org/tenacityteam/libmad/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A high-quality MPEG audio decoder."

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF"
if [ "${TARGET_ARCH}" = "x86_64" ]; then
  PKG_CMAKE_OPTS_TARGET+=" -DOPTIMIZE=ACCURACY"
fi
