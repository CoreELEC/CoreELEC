# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="RTL8821CU"
PKG_VERSION="0278eaa9937a7c60f2916b5d334e40dfb7870bb0"
PKG_SHA256="5ba1950081183e24860c87aa903fc4ee532805df39c7e729c0892bcc319e211f"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/whitebatman2/rtl8821CU"
PKG_URL="https://github.com/whitebatman2/rtl8821CU/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="rtl8821CU-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_SECTION="driver"
PKG_LONGDESC="Realtek RTL8821CU Linux driver"
PKG_IS_KERNEL_PKG="yes"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  make \
       ARCH=$TARGET_KERNEL_ARCH \
       KSRC=$(kernel_path) \
       CROSS_COMPILE=$TARGET_KERNEL_PREFIX \
       CONFIG_POWER_SAVING=n
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    cp *.ko $INSTALL/$(get_full_module_dir)/$PKG_NAME
}
