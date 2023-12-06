# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-dosbox-pure"
PKG_VERSION="ec951632d4cbf015e2c520128b31f2714679bd89"
PKG_SHA256="d805fc1132c7d825564ce13d5f419815791141eef9ccdc28d8b2370f32f8fc20"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/dosbox-pure"
PKG_URL="https://github.com/libretro/dosbox-pure/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="DOSBox Pure is a fork of DOSBox, an emulator for DOS games, built for RetroArch/Libretro aiming for simplicity and ease of use."
PKG_TOOLCHAIN="make"

PKG_LIBNAME="dosbox_pure_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="DOSBOX-PURE_LIB"

PKG_BUILD_FLAGS="+pic"

make_target() {
  # remove optimization from CFLAGS, set via Makefile
  CFLAGS="${CFLAGS//-O3/}"
  CFLAGS="${CFLAGS//-O2/}"
  make CXX=${CXX} CPUFLAGS="${CFLAGS}"
}

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" > ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
