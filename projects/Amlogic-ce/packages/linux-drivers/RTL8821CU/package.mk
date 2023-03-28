# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="RTL8821CU"
PKG_VERSION="5a39cefab32093e3748aa0ac5fa3346a0d35ad37"
PKG_SHA256="9178cd7d96d0861cb35b0c4f74bd795863ea378deea099715c944fb200b0bd4f"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/morrownr/8821cu-20210916"
PKG_URL="https://github.com/morrownr/8821cu-20210916/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="Realtek RTL8821CU Linux driver"
PKG_IS_KERNEL_PKG="yes"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  make V=1 \
       ARCH=$TARGET_KERNEL_ARCH \
       KSRC=$(kernel_path) \
       CROSS_COMPILE=$TARGET_KERNEL_PREFIX \
       CONFIG_POWER_SAVING=n
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    cp *.ko $INSTALL/$(get_full_module_dir)/$PKG_NAME
}
