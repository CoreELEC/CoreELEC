# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-opera"
PKG_VERSION="1eee72f640e4da6f1b8ca68f70b51db22cc474c8"
PKG_SHA256="689650ddbfcd5232e08b5def5346cf68614ad83383658d3e542bf62a8ee8f6bb"
PKG_LICENSE="LGPL with additional notes"
PKG_SITE="https://github.com/libretro/opera-libretro"
PKG_URL="https://github.com/libretro/opera-libretro/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Port of 4DO/libfreedo to libretro."
PKG_TOOLCHAIN="make"

PKG_LIBNAME="opera_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="OPERA_LIB"

pre_make_target() {
  PKG_MAKE_OPTS_TARGET="CC=${CC} CXX=${CXX} AR=${AR}"
}

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
