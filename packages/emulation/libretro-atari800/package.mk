# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-atari800"
PKG_VERSION="d1d0d425458e6b5a2e51ad5f1507bdf97e857c95"
PKG_SHA256="250291312ca10a20643699f8ac506766cad5792e70fc924818a496bfe7ce11cb"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/libretro/libretro-atari800"
PKG_URL="https://github.com/libretro/libretro-atari800/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Libretro port of Atari800 emulator version 3.1.0"
PKG_TOOLCHAIN="make"

PKG_LIBNAME="atari800_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="ATARI800_LIB"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
