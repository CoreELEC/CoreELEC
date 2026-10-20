# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-lrps2"
PKG_VERSION="d4908f40fb32f43c5055852ecaed3c52aa725dce"
PKG_SHA256="445fba62dadfb0427cdb544970fea5c446858a62776784088a1162151095555b"
PKG_ARCH="x86_64"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://github.com/libretro/ps2"
PKG_URL="https://github.com/libretro/ps2/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="LRPS2 is a libretro port of the PCSX2 PlayStation 2 emulator."
PKG_TOOLCHAIN="cmake"

PKG_LIBNAME="pcsx2_libretro.so"
PKG_LIBPATH="bin/${PKG_LIBNAME}"
PKG_LIBVAR="LRPS2_LIB"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
