# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-gambatte"
PKG_VERSION="e6bcb9f43c13f44fbf4a30153e3885b3dd174443"
PKG_SHA256="08e4c84b7bb4d8c31ea49789a0349a6b773cfa05be62d7cba730f9c00fee684b"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/gambatte-libretro"
PKG_URL="https://github.com/libretro/gambatte-libretro/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libretro implementation of libgambatte, an open-source Game Boy Color emulator written for fun and made available in the hope that it will be useful."
PKG_TOOLCHAIN="make"

PKG_LIBNAME="gambatte_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="GAMBATTE_LIB"

PKG_MAKE_OPTS_TARGET="-f Makefile.libretro"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" > ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
