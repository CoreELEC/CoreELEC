# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017-2018 Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="u-boot-Odroid_C2"
PKG_SITE="https://www.denx.de/wiki/U-Boot"
PKG_DEPENDS_TARGET="toolchain gcc-linaro-aarch64-elf:host gcc-linaro-arm-eabi:host"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_LONGDESC="Das U-Boot is a cross-platform bootloader for embedded systems."

PKG_VERSION="ad3e8b1cfb74d22777cb002f01ac992abe457aad"
PKG_URL="https://github.com/CoreELEC/u-boot/archive/$PKG_VERSION.tar.gz"
PKG_SHA256="11f104ebda169f869dfc61b287d3da2faf426f13da438e650c4109d12271a7e2"
PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET u-boot_firmware"
PKG_UBOOT_CONFIG="odroidc2_defconfig"
PKG_TOOLCHAIN="manual"

pre_make_target() {
  cp -r $(get_build_dir u-boot_firmware)/* $PKG_BUILD
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
