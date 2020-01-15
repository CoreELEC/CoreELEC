# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="bl301"
PKG_VERSION="4ad6f7920bd9412aa758e432d82761c7642ac7ce"
PKG_SHA256="a19f20f95074cda71fc6dc46c47eb083d76ac27ce7d217acf1e8807393ec2744"
PKG_LICENSE="GPL"
PKG_SITE="https://coreelec.org"
PKG_URL="https://github.com/CoreELEC/bl301/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain gcc-linaro-aarch64-elf:host gcc-linaro-arm-eabi:host"
PKG_LONGDESC="Das U-Boot is a cross-platform bootloader for embedded systems."
PKG_TOOLCHAIN="manual"

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
}
