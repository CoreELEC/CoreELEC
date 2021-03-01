# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="RTL8814AU"
PKG_VERSION="a0c44795f06d846b656ebac3310ba8f80f41333d"
PKG_SHA256="b4da15f5160cf402d6b2d274cb8bc9c77ca4b533df63a76c9109fe97233cec03"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/tpircher/rtl8814AU"
PKG_URL="https://github.com/tpircher/rtl8814AU/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="rtl8814AU-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_SECTION="driver"
PKG_SHORTDESC="Realtek RTL8814AU Linux drivers"
PKG_LONGDESC="Realtek RTL8814AU Linux drivers"
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
