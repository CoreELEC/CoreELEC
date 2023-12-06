# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-beetle-supergrafx"
PKG_VERSION="81e08f0a81f443eddacfe58557d1534c50ecd6a3"
PKG_SHA256="1c226f290e7f0865ad5c6da191aafb91009c3c02b8c5a21743ba20210b92f622"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/beetle-supergrafx-libretro"
PKG_URL="https://github.com/libretro/beetle-supergrafx-libretro/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Standalone port of Mednafen PCE Fast to libretro."
PKG_TOOLCHAIN="make"

PKG_LIBNAME="mednafen_supergrafx_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="BEETLE-SUPERGRAFX_LIB"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" > ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
