# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dtc"
PKG_VERSION="1.8.1"
PKG_SHA256="23526015a6f1550e0541a53fe7acea1b5a11e3697cdf3a3bdc076abc38f6045d"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://git.kernel.org/pub/scm/utils/dtc/dtc.git/"
PKG_URL="https://www.kernel.org/pub/software/utils/dtc/dtc-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="make:host meson:host flex:host ninja:host zlib:host"
PKG_DEPENDS_TARGET="make:host meson:host gcc:host ninja:host zlib"
PKG_LONGDESC="The Device Tree Compiler"

PKG_MESON_OPTS_TARGET="-Ddefault_library=static -Dtests=false"
PKG_MESON_OPTS_HOST="-Dtests=false"

post_make_host() {
  safe_remove ${PKG_BUILD}/.${HOST_NAME}/libfdt/libfdt.so.*.p
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp -P ${PKG_BUILD}/.${HOST_NAME}/dtc ${TOOLCHAIN}/bin
  mkdir -p ${TOOLCHAIN}/lib
    cp -P ${PKG_BUILD}/.${HOST_NAME}/libfdt/libfdt.so* ${TOOLCHAIN}/lib
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/dtc ${INSTALL}/usr/bin
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/fdtput ${INSTALL}/usr/bin/
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/fdtget ${INSTALL}/usr/bin
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/fdtdump ${INSTALL}/usr/bin/
  mkdir -p ${INSTALL}/usr/{include,lib}
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/libfdt/libfdt.a ${SYSROOT_PREFIX}/usr/lib
    cp -P ${PKG_BUILD}/libfdt/*.h ${SYSROOT_PREFIX}/usr/include
}
