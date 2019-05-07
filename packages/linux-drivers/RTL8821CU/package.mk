# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="RTL8821CU"
PKG_VERSION="fb7fd86822da12522499f01df34f980e5cb337e5"
PKG_SHA256="c29424be8f5c241444228c0cb2c8dd2db086420b77ff21a78d207ddf51302636"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/smp79/rtl8821CU"
PKG_URL="https://github.com/smp79/rtl8821CU/archive/$PKG_VERSION.tar.gz"
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
