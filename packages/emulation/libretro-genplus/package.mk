# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-genplus"
PKG_VERSION="f18eea1ef3cce66c50e556138b41579d2ea08ad3"
PKG_SHA256="d0ee73a23497cc3ceadc0afc2097f681a13bdb0b315553a5fbb44f129bebd414"
PKG_LICENSE="Non-commercial"
PKG_SITE="https://github.com/ekeeke/Genesis-Plus-GX"
PKG_URL="https://github.com/libretro/Genesis-Plus-GX/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Genesis Plus GX is an open-source & portable Sega Mega Drive / Genesis emulator, now also emulating SG-1000, Master System, Game Gear and Sega/Mega CD hardware."
PKG_TOOLCHAIN="make"

PKG_LIBNAME="genesis_plus_gx_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="GENPLUS_LIB"

PKG_MAKE_OPTS_TARGET="-f Makefile.libretro"

if [ "${ARCH}" = "aarch64" ]; then
  PKG_MAKE_OPTS_TARGET+=" NO_OPTIMIZE=1"
fi

pre_make_target() {
  if [ "${ARCH}" = "arm" -o "${ARCH}" = "aarch64" ]; then
    CFLAGS+=" -DALIGN_LONG"
  fi
}

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" > ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
