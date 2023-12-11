# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-handy"
PKG_VERSION="0559d3397f689ea453b986311aeac8dbd33afb0b"
PKG_SHA256="49323a78e8aed122552479945a0eeb0de3052342f3c93b9bfc16edf39f8d2237"
PKG_LICENSE="Zlib"
PKG_SITE="https://github.com/libretro/libretro-handy"
PKG_URL="https://github.com/libretro/libretro-handy/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="K. Wilkins' Atari Lynx emulator Handy for libretro"
PKG_TOOLCHAIN="make"

PKG_LIBNAME="handy_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="HANDY_LIB"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" > ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
