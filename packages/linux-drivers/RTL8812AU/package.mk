# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="RTL8812AU"
PKG_VERSION="4f645eec1718433fbf7f47665266dc302e88d46c"
PKG_SHA256="451b62537bd21601b8d8d8415b62b45e3007704690b7d1b6bf0acd458ab11248"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/aircrack-ng/rtl8812au"
PKG_URL="https://github.com/aircrack-ng/rtl8812au/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Realtek RTL8812AU Linux driver"
PKG_IS_KERNEL_PKG="yes"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  make V=1 \
       ARCH=${TARGET_KERNEL_ARCH} \
       KSRC=$(kernel_path) \
       CROSS_COMPILE=${TARGET_KERNEL_PREFIX} \
       CONFIG_POWER_SAVING=n
}

makeinstall_target() {
  mkdir -p ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
    cp *.ko ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
}
