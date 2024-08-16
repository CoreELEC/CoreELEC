# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2024-present Team CoreELEC (https://coreelec.org)

PKG_NAME="mt7668-wifi-bt"
PKG_VERSION="b26fde565cdf7ab71d8d68ee747ead5e1c9bf78f"
PKG_SHA256="9b9ff0a18b2c883d43f68d321bb900955592b81b3a44d4f09b3b2da843c43574"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/noob404yt/mt7668-wifi-bt"
PKG_URL="https://github.com/CoreELEC/MT7668/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="WiFi & Bluetooth Drivers for MT7668"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

make_target() {
  cd ${PKG_BUILD}/MT7668-Bluetooth
    kernel_make EXTRA_CFLAGS="-w" \
      KERNEL_SRC=$(kernel_path)

  cd ${PKG_BUILD}/MT7668-WiFi
    kernel_make EXTRA_CFLAGS="-w" \
      KERNELDIR=$(kernel_path)
}

makeinstall_target() {
  mkdir -p ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
    find ${PKG_BUILD}/ -name \*.ko -not -path '*/\.*' -exec cp {} ${INSTALL}/$(get_full_module_dir)/${PKG_NAME} \;

  mkdir -p ${INSTALL}/$(get_full_firmware_dir)
    cp ${PKG_BUILD}/MT7668-WiFi/7668_firmware/* ${INSTALL}/$(get_full_firmware_dir)
}
