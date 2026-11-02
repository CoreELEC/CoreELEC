# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="RTL8852BS-aml"
PKG_VERSION="40dc73293ded42d373677262d6a904ff6f1efa2d"
PKG_SHA256=""
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/Stane1983/rtl8852bs-aml"
PKG_URL="https://github.com/CoreELEC/RTL8852BS-aml/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="Realtek RTL8852BS Linux driver"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

make_target() {
  kernel_make -C ${PKG_BUILD} \
    M=${PKG_BUILD} \
    KSRC=$(kernel_path) \
    CONFIG_POWER_SAVE=n \
    CONFIG_RTW_DEBUG=n \
    modules
}

makeinstall_target() {
  mkdir -p ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
  find ${PKG_BUILD}/ -name \*.ko -not -path '*/\.*' -exec cp {} ${INSTALL}/$(get_full_module_dir)/${PKG_NAME} \;
}
