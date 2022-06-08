# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="uwe5631-aml"
PKG_VERSION="fc30b0d910b3cf26f37cebb649fff0665d79281e"
PKG_SHA256=""
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/Stane1983/uwe5631-aml"
PKG_URL="https://github.com/Stane1983/uwe5631-aml/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="uwe5631-aml: Unisoc UWE5621 WIFI/BT driver"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

make_target() {
  kernel_make -C ${PKG_BUILD} \
       M=${PKG_BUILD} \
       KERNEL_SRC=$(kernel_path) \
       EXTRA_CFLAGS="-fno-pic -Wno-sizeof-pointer-memaccess -I${PKG_BUILD}/BSP/include" \
       modules
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME

  find $PKG_BUILD/ -name \*.ko -not -path '*/\.*' \
    -exec cp {} $INSTALL/$(get_full_module_dir)/$PKG_NAME \;
}
