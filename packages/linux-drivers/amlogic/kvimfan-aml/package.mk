# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="kvimfan-aml"
PKG_VERSION="1.0"
PKG_REV="101"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE=""
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="driver"
PKG_SECTION="service/system"
PKG_SHORTDESC="Khadas VIM2 fan control service"
PKG_LONGDESC="Khadas VIM2 fan control service"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  kernel_make -C "$(kernel_path)" M="$PKG_BUILD"

}
makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    find $PKG_BUILD/ -name \*.ko -not -path '*/\.*' -exec cp {} $INSTALL/$(get_full_module_dir)/$PKG_NAME \;

}
post_install() {
  enable_service kvimfan.service
}
