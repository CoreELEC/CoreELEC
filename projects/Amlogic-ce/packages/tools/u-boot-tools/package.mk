# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="u-boot-tools"
PKG_VERSION="2016.03"
PKG_SHA256="53de9a002f9e8ff198d3daaa82e778da63d75ac8b63bb8a4e0bb2cf63391e7a7"
PKG_SITE="https://u-boot-project.org"
PKG_URL="https://git.u-boot-project.org/u-boot/u-boot/-/archive/v${PKG_VERSION}/u-boot-v${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_HOST="gcc:host"
PKG_DEPENDS_TARGET="toolchain u-boot-tools:host"
PKG_DEPENDS_INIT="toolchain u-boot-tools"
PKG_LICENSE="GPL"
PKG_LONGDESC="Das U-Boot is a cross-platform bootloader for embedded systems."

make_host() {
  make mrproper
  make dummy_defconfig
  make tools-only
}

make_target() {
  CROSS_COMPILE="${TARGET_PREFIX}" LDFLAGS="" ARCH=arm make dummy_defconfig
  CROSS_COMPILE="${TARGET_PREFIX}" LDFLAGS="" ARCH=arm make env
}

make_init() {
  : # reuse make_target()
}

makeinstall_init() {
  mkdir -p ${INSTALL}/etc
    cp ${PKG_DIR}/config/fw_env.config ${INSTALL}/etc/fw_env.config

  mkdir -p ${INSTALL}/usr/sbin
    cp tools/env/fw_printenv ${INSTALL}/usr/sbin/fw_setenv
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp tools/mkimage ${TOOLCHAIN}/bin
}

makeinstall_target() {
  mkdir -p ${INSTALL}/etc
    cp ${PKG_DIR}/config/fw_env.config ${INSTALL}/etc/fw_env.config

  mkdir -p ${INSTALL}/usr/sbin
    cp tools/env/fw_printenv ${INSTALL}/usr/sbin/fw_printenv
    cp tools/env/fw_printenv ${INSTALL}/usr/sbin/fw_setenv
}
