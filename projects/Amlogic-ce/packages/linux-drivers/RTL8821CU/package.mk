# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="RTL8821CU"
PKG_VERSION="377d24a336c491f3c0c0a92eb4088ad06b55a956"
PKG_SHA256="0ef895c5e8d6aba1c523dc7fbffdfcf4a08982d37e3690eb3c3fd9bd40c11d4d"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/morrownr/8821au-20210708"
PKG_URL="https://github.com/morrownr/8821au-20210708/archive/$PKG_VERSION.tar.gz"
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
