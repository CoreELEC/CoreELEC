# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-vbam"
PKG_VERSION="146cf91d43d1cc1e7616486e191a4e1780d94aa2"
PKG_SHA256="90b23eb8e69b65f51bf4f2230a09729ed6a4e8a2b1d511f642f4ac794e7c7811"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/visualboyadvance-m/visualboyadvance-m"
PKG_URL="https://github.com/visualboyadvance-m/visualboyadvance-m/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="VBA-M with libretro integration"
PKG_TOOLCHAIN="make"

PKG_LIBNAME="vbam_libretro.so"
PKG_LIBPATH="../src/libretro/${PKG_LIBNAME}"
PKG_LIBVAR="VBAM_LIB"

PKG_MAKE_OPTS_TARGET="-C ../src/libretro/"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
