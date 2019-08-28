# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="u-boot-Khadas_VIM3"
PKG_VERSION="9b7ee287cf63280685fe4a7e797928e79db27be1"
PKG_SHA256="2af9a49e85d0fd22d07b8758749973dd0ad8c6fa54c153651f4918faa86ca4b9"
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
  CROSS_COMPILE=aarch64-elf- make kvim3_defconfig
  CROSS_COMPILE=aarch64-elf- make
}

makeinstall_target() {
  : # nothing
}
