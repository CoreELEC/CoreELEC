# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-vecx"
PKG_VERSION="eacee1f6f029688b043ed802cece29dd3c320e21"
PKG_SHA256="f8dfd3da427b101b8d3e4fb187eea3bb326ad924942d3c09f3388fc879e08ba3"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/libretro/libretro-vecx"
PKG_URL="https://github.com/libretro/libretro-vecx/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libretro adaptation of vecx"
PKG_TOOLCHAIN="make"

PKG_LIBNAME="vecx_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="VECX_LIB"

PKG_MAKE_OPTS_TARGET="-f Makefile.libretro"

if [ "${OPENGL_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGL}"
fi

if [ "${OPENGL_SUPPORT}" = "yes" ] &&
   [ "${DISPLAYSERVER}" != "x11" ]; then
  PKG_MAKE_OPTS_TARGET+=" USE_GLVND=1"
fi

if [ "${OPENGLES_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGLES}"
fi

if [ "${VULKAN_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${VULKAN}"
fi

if [ "${PROJECT}" = "RPi" ]; then
  if [ "${OPENGLES}" = "mesa" ]; then
    PKG_MAKE_OPTS_TARGET+=" platform=rpi-mesa"
  else
    PKG_MAKE_OPTS_TARGET+=" platform=rpi"
  fi
elif [ "${OPENGLES_SUPPORT}" = "yes" ]; then
  PKG_MAKE_OPTS_TARGET+=" HAS_GLES=1 GLES=1"
fi

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp ${PKG_LIBPATH} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
