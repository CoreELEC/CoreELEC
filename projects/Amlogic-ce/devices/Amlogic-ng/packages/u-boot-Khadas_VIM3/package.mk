# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="u-boot-Khadas_VIM3"
PKG_VERSION="07b8e83a020cf2e1886e3762aed2be33d6e31cb3"
PKG_SHA256="95e61d36310b1fa89594144879744c72762cfb2412731c8a785104dcc77516fc"
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
