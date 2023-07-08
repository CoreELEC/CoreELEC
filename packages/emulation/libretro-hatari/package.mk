# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-hatari"
PKG_VERSION="00a46e1ed216e3b51b1cd829e04a36caf30a0338"
PKG_SHA256="06a745a1094c46320a1980d5c0f69bf04481e169b56179021bf59f549252f2a8"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/hatari"
PKG_URL="https://github.com/libretro/hatari/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="New rebasing of Hatari based on Mercurial upstream. Tries to be a shallow fork for easy upstreaming later on."
PKG_TOOLCHAIN="make"

PKG_LIBNAME="hatari_libretro.so"
PKG_LIBPATH="../${PKG_LIBNAME}"
PKG_LIBVAR="HATARI_LIB"

PKG_MAKE_OPTS_TARGET="-C ../ -f Makefile.libretro"

pre_make_target() {
  if [ "${ARCH}" = "arm" ]; then
    CFLAGS+=" -DNO_ASM -DARM -D__arm__ -DARM_ASM -DNOSSE -DARM_HARDFP"
  fi
}

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" > ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
