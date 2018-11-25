# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="u-boot-LePotato"
PKG_SITE="https://www.denx.de/wiki/U-Boot"
PKG_DEPENDS_TARGET="toolchain gcc-linaro-aarch64-elf:host gcc-linaro-arm-eabi:host"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_LONGDESC="Das U-Boot is a cross-platform bootloader for embedded systems."

PKG_VERSION="488e3d3a1f19e8798b9e4afcd5e3152bc7b42e4b"
PKG_SHA256="68355d48bdc0687f43d8240f920dd49d70856e135977f222d1056b898e4b06ea"
PKG_URL="https://github.com/CoreELEC/u-boot/archive/$PKG_VERSION.tar.gz"
PKG_UBOOT_CONFIG="libretech_cc_defconfig"
PKG_TOOLCHAIN="manual"

pre_make_target() {
  sed -i "s|arm-none-eabi-|arm-eabi-|g" $PKG_BUILD/Makefile $PKG_BUILD/arch/arm/cpu/armv8/gx*/firmware/scp_task/Makefile 2>/dev/null || true
}

make_target() {
  [ "${BUILD_WITH_DEBUG}" = "yes" ] && PKG_DEBUG=1 || PKG_DEBUG=0
  export PATH=$TOOLCHAIN/lib/gcc-linaro-aarch64-elf/bin/:$TOOLCHAIN/lib/gcc-linaro-arm-eabi/bin/:$PATH
  DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make mrproper
  DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make $PKG_UBOOT_CONFIG
  DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make HOSTCC="$HOST_CC" HOSTSTRIP="true"
}

makeinstall_target() {
    : # nothing
}
