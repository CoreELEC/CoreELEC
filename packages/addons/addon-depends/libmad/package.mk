# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2014 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libmad"
PKG_VERSION="be34ec9fe47577e7f3d84cc9640d2a4696d478d6"
PKG_SHA256="478d2e3ef4307b0731cc43eca917eba9689285e693a84381d83d0ef81177f05a"
PKG_LICENSE="GPL"
PKG_SITE="http://www.mars.org/home/rob/proj/mpeg/"
PKG_URL="https://codeberg.org/tenacityteam/libmad/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A high-quality MPEG audio decoder."

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF"
if [ "${TARGET_ARCH}" = "x86_64" ]; then
  PKG_CMAKE_OPTS_TARGET+=" -DOPTIMIZE=ACCURACY"
fi
