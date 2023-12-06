# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-bsnes"
PKG_VERSION="c4b27b3ae44a712dbe90e159bdba43df0c9f57de"
PKG_SHA256="bca29e7d611ba3649b7653a76b9443d52057597d4102d84d9c7485dfd45cf20b"
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
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" > ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
