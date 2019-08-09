# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="u-boot-Khadas_VIM3"
PKG_VERSION="b7e2b85a6ec04ee6623357bab837b8a00368d924"
PKG_SHA256="e5719071d54d4b4d59d661f56a855e487b52227ab02b3f5791f1940037baa38e"
PKG_LICENSE="GPL"
PKG_SITE="https://www.denx.de/wiki/U-Boot"
PKG_URL="https://github.com/khadas/u-boot/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain gcc-linaro-aarch64-elf:host gcc-linaro-arm-eabi:host"
PKG_LONGDESC="Das U-Boot is a cross-platform bootloader for embedded systems."
PKG_TOOLCHAIN="manual"

pre_make_target() {
  unset LDFLAGS
  unset CFLAGS
}

make_target() {
  export PATH=$TOOLCHAIN/lib/gcc-linaro-aarch64-elf/bin/:$TOOLCHAIN/lib/gcc-linaro-arm-eabi/bin/:$PATH
  CROSS_COMPILE=aarch64-elf- ./mk kvim3 --systemroot
}

makeinstall_target() {
  : # nothing
}
