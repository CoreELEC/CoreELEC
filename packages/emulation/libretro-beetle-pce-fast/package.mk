# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-beetle-pce-fast"
PKG_VERSION="52675734da114a19b3705f03906b1455f3d76644"
PKG_SHA256="783a2cdb8c94ff7194fefc415a04c3a75b24a79cbac1ca93ace946810cd57002"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/beetle-pce-fast-libretro"
PKG_URL="https://github.com/libretro/beetle-pce-fast-libretro/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Standalone port of Mednafen PCE Fast to libretro."
PKG_TOOLCHAIN="make"

PKG_MAKE_OPTS_TARGET="HAVE_CDROM=1"

PKG_LIBNAME="mednafen_pce_fast_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="BEETLE-PCE-FAST_LIB"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
