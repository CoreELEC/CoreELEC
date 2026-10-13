# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2014 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libmad"
PKG_VERSION="0.16.5"
PKG_SHA256="e32320a010e3cc00bfa66e0cde523a969313cddfd81e07ec4e3ef06d10ad3fe0"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="http://www.mars.org/home/rob/proj/mpeg/"
PKG_URL="https://codeberg.org/tenacityteam/libmad/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A high-quality MPEG audio decoder."
PKG_BUILD_FLAGS="-sysroot"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -DEXAMPLE=OFF"

if [ "${TARGET_ARCH}" = "x86_64" ]; then
  PKG_CMAKE_OPTS_TARGET+=" -DOPTIMIZE=ACCURACY"
fi
