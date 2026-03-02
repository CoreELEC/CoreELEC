# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-2048"
PKG_VERSION="e70c3f82d2b861c64943aaff7fcc29a63013997d"
PKG_SHA256="07d51892d173d85715d0cddd5963c50c2308e2d92401c4e79208544bc95f6f19"
PKG_LICENSE="Public domain"
PKG_SITE="https://github.com/libretro/libretro-2048"
PKG_URL="https://github.com/libretro/libretro-2048/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Port of 2048 puzzle game to the libretro API."
PKG_TOOLCHAIN="make"
PKG_MAKE_OPTS_TARGET="-f Makefile.libretro"

PKG_LIBNAME="2048_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="2048_LIB"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
