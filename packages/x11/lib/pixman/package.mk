# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pixman"
PKG_VERSION="0.43.2"
PKG_SHA256="b43dc9549c02c598fb11321d6fca47151f739a076c73fcd8971b5c023a06949e"
PKG_LICENSE="OSS"
PKG_SITE="https://www.x.org/"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="toolchain:host"
PKG_DEPENDS_TARGET="toolchain util-macros"
PKG_LONGDESC="Pixman is a generic library for manipulating pixel regions, contains low-level pixel manipulation routines."

if [ "${TARGET_ARCH}" = arm ]; then
  if target_has_feature neon; then
    PIXMAN_NEON="-Dneon=enabled"
  else
    PIXMAN_NEON="-Dneon=disabled"
  fi
  PIXMAN_CONFIG="-Dmmx=disabled -Dsse2=disabled -Dvmx=disabled -Darm-simd=enabled ${PIXMAN_NEON} -Diwmmxt=disabled"
elif [ "${TARGET_ARCH}" = aarch64 ]; then
  PIXMAN_CONFIG="-Dmmx=disabled -Dsse2=disabled -Dvmx=disabled -Darm-simd=disabled -Dneon=disabled -Diwmmxt=disabled"
elif [ "${TARGET_ARCH}" = x86_64  ]; then
  PIXMAN_CONFIG="-Dmmx=enabled -Dsse2=enabled -Dssse3=disabled -Dvmx=disabled -Darm-simd=disabled -Dneon=disabled"
fi

PKG_MESON_OPTS_HOST="-Dgtk=disabled \
                     -Dtests=disabled \
                     -Ddemos=disabled"

PKG_MESON_OPTS_TARGET="-Dopenmp=disabled \
                       -Dloongson-mmi=disabled \
                       ${PIXMAN_CONFIG} \
                       -Dmips-dspr2=disabled \
                       -Dgnu-inline-asm=enabled \
                       -Dtimers=false \
                       -Dgtk=disabled \
                       -Dlibpng=disabled \
                       -Dtests=disabled \
                       -Ddemos=disabled"

post_makeinstall_target() {
  cp ${SYSROOT_PREFIX}/usr/lib/pkgconfig/pixman-1.pc \
     ${SYSROOT_PREFIX}/usr/lib/pkgconfig/pixman.pc
  cp -rf ${SYSROOT_PREFIX}/usr/include/pixman-1 \
     ${SYSROOT_PREFIX}/usr/include/pixman
}
