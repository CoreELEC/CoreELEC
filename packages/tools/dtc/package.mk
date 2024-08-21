# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dtc"
PKG_VERSION="1.7.1"
PKG_SHA256="398098bac205022b39d3dce5982b98c57f1023f3721a53ebcbb782be4cf7885e"
PKG_LICENSE="GPL"
PKG_SITE="https://git.kernel.org/pub/scm/utils/dtc/dtc.git/"
PKG_URL="https://www.kernel.org/pub/software/utils/dtc/dtc-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="make:host flex:host ninja:host"
PKG_DEPENDS_TARGET="make:host gcc:host ninja:host"
PKG_LONGDESC="The Device Tree Compiler"
PKG_TOOLCHAIN="make"

PKG_MAKE_OPTS_TARGET="dtc fdtput fdtget libfdt"
PKG_MAKE_OPTS_HOST="dtc libfdt"

pre_make_host() {
  mkdir -p ${PKG_BUILD}/.${HOST_NAME}
    cp -a ${PKG_BUILD}/* ${PKG_BUILD}/.${HOST_NAME}

  cd ${PKG_BUILD}/.${HOST_NAME}
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp -P ${PKG_BUILD}/.${HOST_NAME}/dtc ${TOOLCHAIN}/bin
  mkdir -p ${TOOLCHAIN}/lib
    cp -P ${PKG_BUILD}/.${HOST_NAME}/libfdt/libfdt.so* ${TOOLCHAIN}/lib
}

pre_make_target() {
  mkdir -p ${PKG_BUILD}/.${TARGET_NAME}
    cp -a ${PKG_BUILD}/* ${PKG_BUILD}/.${TARGET_NAME}

  cd ${PKG_BUILD}/.${TARGET_NAME}
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
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/libfdt/*.h ${SYSROOT_PREFIX}/usr/include/
}
