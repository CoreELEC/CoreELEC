# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="bl301"
PKG_VERSION="a824623258d3f92b89303f6ae4e4d1047022c032"
PKG_SHA256="b451dd91649a6e6ecc689bc42e563491be39d292fd383bf0aba988499c30d6e8"
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

  for PKG_BL301_SUBDEVICE in ${BL301_SUBDEVICES}; do
    PKG_UBOOT_CONFIG=${PKG_BL301_SUBDEVICE}_defconfig

    if [[ -f "${PKG_BUILD}/configs/${PKG_UBOOT_CONFIG,,}" ]]; then
      echo Building bl301 for ${PKG_BL301_SUBDEVICE}
      DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make ${PKG_UBOOT_CONFIG,,}
      DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make HOSTCC="${HOST_CC}" HOSTSTRIP="true" bl301.bin
      mv ${PKG_BUILD}/build/scp_task/bl301.bin ${PKG_BUILD}/build/${PKG_BL301_SUBDEVICE}_bl301.bin
      echo "moved to: " ${PKG_BUILD}/build/${PKG_BL301_SUBDEVICE}_bl301.bin
    fi
  done
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/share/bootloader/bl301

  for PKG_BL301_SUBDEVICE in ${BL301_SUBDEVICES}; do
    PKG_BIN=${PKG_BUILD}/build/${PKG_BL301_SUBDEVICE}_bl301.bin
    cp -av ${PKG_BIN} ${INSTALL}/usr/share/bootloader/bl301/${PKG_BL301_SUBDEVICE}_bl301.bin
  done
}
