# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="RTL8192DU"
PKG_VERSION="a82c83ec8d1363a8bd1e312378c1ba47971d0012"
PKG_SHA256="be5ee40f289332b4eb9ca16a978dae5117ae2734b7292229dca09599df45b542"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/lwfinger/rtl8192du"
PKG_URL="https://github.com/lwfinger/rtl8192du/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Realtek RTL8192DU Linux driver"
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
