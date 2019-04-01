# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-2019 Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="RTL8812AU"
PKG_VERSION="66cc784891ecd3c49cdad11851833b6609cc72d7"
PKG_SHA256="12bc7871b73d1839c1c0791d2b8ed748715708204387ce9bfa7ef6c22a4ae613"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/codewalkerster/android_hardware_wifi_realtek_drivers_rtl8812au"
PKG_URL="https://github.com/codewalkerster/android_hardware_wifi_realtek_drivers_rtl8812au/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="Realtek RTL8812AU Linux driver"
PKG_IS_KERNEL_PKG="yes"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  make -C $(kernel_path) M=$PKG_BUILD \
       ARCH=$TARGET_KERNEL_ARCH \
       KSRC=$(kernel_path) \
       CROSS_COMPILE=$TARGET_KERNEL_PREFIX
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    cp *.ko $INSTALL/$(get_full_module_dir)/$PKG_NAME
}
