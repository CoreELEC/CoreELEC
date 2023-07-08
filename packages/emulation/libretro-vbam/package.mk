# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-vbam"
PKG_VERSION="790618102dea1a4c94a115e4f3e8b432eb53adee"
PKG_SHA256="cbfdcc2e7bb440c4cdc69f85ee1c6fe23b3cbc37375862f26a351f7db8583b00"
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
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" > ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
