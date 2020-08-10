# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="bl301"
PKG_VERSION="ea57d6f18b6cad4d92c1f1d598e9881ec89e87c1"
PKG_SHA256="4202bc3cf7fef72f657fc2a5d02d884d91f9a7ba6a4b1d1bec4fa95ff10dddd2"
PKG_LICENSE="GPL"
PKG_SITE="https://coreelec.org"
PKG_URL="https://github.com/CoreELEC/bl301/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain gcc-linaro-aarch64-elf:host gcc-linaro-arm-eabi:host"
PKG_LONGDESC="Das U-Boot is a cross-platform bootloader for embedded systems."
PKG_TOOLCHAIN="manual"

pre_make_target() {
  sed -i "s|arm-none-eabi-|arm-eabi-|g" $PKG_BUILD/Makefile $PKG_BUILD/arch/arm/cpu/armv8/*/firmware/scp_task/Makefile 2>/dev/null || true
}

make_target() {
  [ "${BUILD_WITH_DEBUG}" = "yes" ] && PKG_DEBUG=1 || PKG_DEBUG=0
  export PATH=$TOOLCHAIN/lib/gcc-linaro-aarch64-elf/bin/:$TOOLCHAIN/lib/gcc-linaro-arm-eabi/bin/:$PATH
  DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make mrproper

  for f in $(find ${PKG_BUILD}/configs -mindepth 1); do
    PKG_UBOOT_CONFIG=$(basename -- "$f")
    PKG_BL301_SUBDEVICE=${PKG_UBOOT_CONFIG%_defconfig}
    echo Building bl301 for ${PKG_BL301_SUBDEVICE}
    DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make ${PKG_UBOOT_CONFIG}
    DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make HOSTCC="${HOST_CC}" HOSTSTRIP="true" bl301.bin
    mv ${PKG_BUILD}/build/scp_task/bl301.bin ${PKG_BUILD}/build/${PKG_BL301_SUBDEVICE}_bl301.bin
    echo "moved blob to: " ${PKG_BUILD}/build/${PKG_BL301_SUBDEVICE}_bl301.bin
    rm -rf ${PKG_BUILD}/build/scp_task
  done
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/share/bootloader/bl301

  for f in $(find ${PKG_BUILD}/configs -mindepth 1); do
    PKG_UBOOT_CONFIG=$(basename -- "$f")
    PKG_BL301_SUBDEVICE=${PKG_UBOOT_CONFIG%_defconfig}
    PKG_BIN=${PKG_BUILD}/build/${PKG_BL301_SUBDEVICE}_bl301.bin
    cp -av ${PKG_BIN} ${INSTALL}/usr/share/bootloader/bl301/${PKG_BL301_SUBDEVICE}_bl301.bin
  done

  [ -d "${PKG_BUILD}/bl30" ] && cp -av ${PKG_BUILD}/bl30 ${INSTALL}/usr/share/bootloader/bl301 || :
  [ -d "${PKG_BUILD}/bl31" ] && cp -av ${PKG_BUILD}/bl31 ${INSTALL}/usr/share/bootloader/bl301 || :
}
