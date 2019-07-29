# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="RTL8822BU"
PKG_VERSION="dea3bb8e631191ded1839c53fb266d80ef7e8ad3"
PKG_SHA256="34474838558a8502edc9bcd1091c8911ec128d50aeda2f83173b36c1a592250c"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/MeissnerEffect/rtl8822bu"
PKG_URL="https://github.com/EntropicEffect/rtl8822bu/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="rtl8822bu-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_SECTION="driver"
PKG_LONGDESC="Realtek RTL8822BU Linux driver"
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
