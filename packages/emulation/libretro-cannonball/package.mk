# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-cannonball"
PKG_VERSION="65cb1f5f227db45abcd9d2006efe7687a0d9cb72"
PKG_SHA256="40b82778641050d25841735f40f45f23643525dbf410fb1a581768f9cbe8ba14"
PKG_LICENSE="LicenseRef-MAME"
PKG_SITE="https://github.com/libretro/cannonball"
PKG_URL="https://github.com/libretro/cannonball/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Cannonball: An Enhanced OutRun Engine"
PKG_TOOLCHAIN="make"

PKG_LIBNAME="cannonball_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="CANNONBALL_LIB"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake

  mkdir -p ${SYSROOT_PREFIX}/usr/share/retroarch/system/cannonball/res/
  cp -v res/{tilemap.bin,tilepatch.bin} docs/license.txt ${SYSROOT_PREFIX}/usr/share/retroarch/system/cannonball/res/
  cp -v roms/roms.txt ${SYSROOT_PREFIX}/usr/share/retroarch/system/cannonball/
}
