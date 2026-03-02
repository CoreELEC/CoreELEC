# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-bsnes"
PKG_VERSION="d0a61b2c679bc73286be5471b222b1f1ebfb67b9"
PKG_SHA256="ad8ee455b8214a7ea88941e29333a26fe4cde8626ce04f95d938ca98c2870127"
PKG_LICENSE="GPLv3/ISC"
PKG_SITE="https://github.com/libretro/bsnes"
PKG_URL="https://github.com/libretro/bsnes-libretro/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Super Nintendo (Super Famicom) emulator"
PKG_TOOLCHAIN="make"

PKG_LIBNAME="bsnes_libretro.so"
PKG_LIBPATH="bsnes/out/${PKG_LIBNAME}"
PKG_LIBVAR="BSNES_LIB"

PKG_MAKE_OPTS_TARGET="-C bsnes -f GNUmakefile target=libretro platform=linux local=false"

pre_make_target() {
  PKG_MAKE_OPTS_TARGET+=" compiler=${CXX}"
}

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
