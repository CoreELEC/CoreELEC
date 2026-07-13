# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="glew"
PKG_VERSION="2.3.1"
PKG_SHA256="b64790f94b926acd7e8f84c5d6000a86cb43967bd1e688b03089079799c9e889"
PKG_LICENSE="MIT"
PKG_SITE="http://glew.sourceforge.net/"
PKG_URL="https://downloads.sourceforge.net/project/glew/glew/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tgz"
PKG_DEPENDS_TARGET="toolchain libX11"
PKG_LONGDESC="A cross-platform C/C++ extension loading library."

make_target() {
  make CC="${CC}" LD="${CC}" AR="${AR}" \
       POPT="${CFLAGS}" LDFLAGS.EXTRA="${LDFLAGS}" \
       GLEW_DEST="/usr" LIBDIR="/usr/lib" lib/libGLEW.a glew.pc
}

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib
    cp -PR lib/libGLEW.a ${SYSROOT_PREFIX}/usr/lib

  mkdir -p ${SYSROOT_PREFIX}/usr/lib/pkgconfig
    cp -PR glew.pc ${SYSROOT_PREFIX}/usr/lib/pkgconfig

  mkdir -p ${SYSROOT_PREFIX}/usr/include
    cp -PR include/GL ${SYSROOT_PREFIX}/usr/include
}
