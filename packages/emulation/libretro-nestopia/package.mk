# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-nestopia"
PKG_VERSION="473d3072be67fa2542ca833c274ef6682cf0f0bc"
PKG_SHA256="0acbb6c53d87721f9065eed6c3b654a06704cbb500290c4e9d82aa0298d16c5b"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/nestopia"
PKG_URL="https://github.com/libretro/nestopia/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Libretro implementation of NEStopia. (Nintendo Entertainment System)"
PKG_TOOLCHAIN="make"

PKG_LIBNAME="nestopia_libretro.so"
PKG_LIBPATH="libretro/${PKG_LIBNAME}"
PKG_LIBVAR="NESTOPIA_LIB"

PKG_MAKE_OPTS_TARGET="-C libretro/"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
