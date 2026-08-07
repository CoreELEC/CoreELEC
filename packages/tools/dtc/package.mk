# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dtc"
PKG_VERSION="1.8.1"
PKG_SHA256="23526015a6f1550e0541a53fe7acea1b5a11e3697cdf3a3bdc076abc38f6045d"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://git.kernel.org/pub/scm/utils/dtc/dtc.git/"
PKG_URL="https://www.kernel.org/pub/software/utils/dtc/dtc-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="make:host flex:host ninja:host"
PKG_DEPENDS_TARGET="make:host gcc:host ninja:host"
PKG_LONGDESC="The Device Tree Compiler"

PKG_MESON_OPTS_TARGET="-Dtests=false
                       -Dpython=disabled"
PKG_MESON_OPTS_HOST="-Dtests=false
                     -Dpython=disabled"

post_make_host() {
  safe_remove ${PKG_BUILD}/.${HOST_NAME}/libfdt/libfdt.so.*.p
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp -P ${PKG_BUILD}/.${HOST_NAME}/dtc ${TOOLCHAIN}/bin
  mkdir -p ${TOOLCHAIN}/lib
    cp -P ${PKG_BUILD}/.${HOST_NAME}/libfdt/libfdt.so* ${TOOLCHAIN}/lib
}

post_make_target() {
  safe_remove ${PKG_BUILD}/.${TARGET_NAME}/libfdt/libfdt.so.*.p
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/dtc ${INSTALL}/usr/bin
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/fdtput ${INSTALL}/usr/bin/
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/fdtget ${INSTALL}/usr/bin/
  mkdir -p ${INSTALL}/usr/lib
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/libfdt/libfdt.so* ${INSTALL}/usr/lib/
  mkdir -p ${SYSROOT_PREFIX}/usr/lib
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/libfdt/libfdt.so* ${SYSROOT_PREFIX}/usr/lib/
  mkdir -p ${SYSROOT_PREFIX}/usr/include
    cp -P ${PKG_BUILD}/libfdt/*.h ${SYSROOT_PREFIX}/usr/include/
}
