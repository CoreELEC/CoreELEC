# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="u-boot-Odroid_N2"
PKG_VERSION="61bd6d6f99c00ab59d714d4979fca90c422dc8b6"
PKG_SHA256="439e416fd9d3efdeb4b2e1d1432d61210410d27990071f8da2066f2d658c8101"
PKG_LICENSE="GPL"
PKG_SITE="https://www.denx.de/wiki/U-Boot"
PKG_URL="https://github.com/CoreELEC/u-boot/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain gcc-linaro-aarch64-elf:host gcc-linaro-arm-eabi:host"
PKG_LONGDESC="Das U-Boot is a cross-platform bootloader for embedded systems."
PKG_TOOLCHAIN="manual"

pre_make_target() {
  sed -i "s|arm-none-eabi-|arm-eabi-|g" $PKG_BUILD/Makefile $PKG_BUILD/arch/arm/cpu/armv8/*/firmware/scp_task/Makefile 2>/dev/null || true
}

configure_package() {
  PKG_UBOOT_CONFIG="odroidn2_defconfig"
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
