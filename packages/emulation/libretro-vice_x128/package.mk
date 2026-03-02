# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-vice_x128"
PKG_VERSION="b3a5d5c6e49a382bb107db9fbe20b9a9f39f96ac"
PKG_SHA256="c54fe2b1a3c01a7402bb2666564a6a073947b5d24be96dd26e686bef44decf6f"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/vice-libretro"
PKG_URL="https://github.com/libretro/vice-libretro/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Versatile Commodore 8-bit Emulator version"
PKG_TOOLCHAIN="manual"

PKG_LIBNAME="vice_x128_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="VICE-X128_LIB"

make_target() {
  make EMUTYPE=x128
}

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
