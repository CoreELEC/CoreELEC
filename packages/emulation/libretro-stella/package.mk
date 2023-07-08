# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-stella"
PKG_VERSION="d47cdb3d3ea4485207d2842c26ec126646bd25f4"
PKG_SHA256="aa4100027248003a88f4b83c3b571c0e42d8cc078552a0f4b3e8e95f4191fcc8"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/stella-emu/stella"
PKG_URL="https://github.com/stella-emu/stella/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Stella is a multi-platform Atari 2600 VCS emulator released under the GNU General Public License (GPL)."
PKG_TOOLCHAIN="make"

PKG_LIBNAME="stella_libretro.so"
PKG_LIBPATH="../src/os/libretro/${PKG_LIBNAME}"
PKG_LIBVAR="STELLA_LIB"

PKG_MAKE_OPTS_TARGET="-C ../src/os/libretro -f Makefile"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" > ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
