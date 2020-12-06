# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dtc"
PKG_VERSION="1.6.0"
PKG_SHA256="af720893485b02441f8812773484b286f969d1b8c98769d435a75c6ad524104b"
PKG_LICENSE="GPL"
PKG_SITE="https://git.kernel.org/pub/scm/utils/dtc/dtc.git/"
PKG_URL="https://git.kernel.org/pub/scm/utils/dtc/dtc.git/snapshot/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="toolchain:host zlib:host"
PKG_DEPENDS_TARGET="toolchain zlib"
PKG_LONGDESC="The Device Tree Compiler"

PKG_MAKE_OPTS_TARGET="dtc fdtput fdtget fdtdump libfdt"
PKG_MAKE_OPTS_HOST="dtc libfdt"

pre_configure_host() {
  export LDLIBS_dtc="-lz"
  export EXTRA_CFLAGS="-I${TOOLCHAIN}/include"
}

pre_make_host() {
  make clean
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp -P ${PKG_BUILD}/dtc ${TOOLCHAIN}/bin
  mkdir -p ${TOOLCHAIN}/lib
    cp -P ${PKG_BUILD}/libfdt/libfdt.so ${TOOLCHAIN}/lib
}

pre_make_target() {
  make clean
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp -P ${PKG_BUILD}/dtc ${INSTALL}/usr/bin
    cp -P ${PKG_BUILD}/fdtput ${INSTALL}/usr/bin/
    cp -P ${PKG_BUILD}/fdtget ${INSTALL}/usr/bin/
    cp -P ${PKG_BUILD}/fdtdump ${INSTALL}/usr/bin/

  # copy to toolchain
  mkdir -p ${SYSROOT_PREFIX}/usr/{include,lib}
    cp -P ${PKG_BUILD}/libfdt/libfdt.a ${SYSROOT_PREFIX}/usr/lib
    cp -P ${PKG_BUILD}/libfdt/fdt.h ${SYSROOT_PREFIX}/usr/include
    cp -P ${PKG_BUILD}/libfdt/libfdt.h ${SYSROOT_PREFIX}/usr/include
    cp -P ${PKG_BUILD}/libfdt/libfdt_env.h ${SYSROOT_PREFIX}/usr/include
}
