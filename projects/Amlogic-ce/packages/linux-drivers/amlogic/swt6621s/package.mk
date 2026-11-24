# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)

PKG_NAME="swt6621s"
PKG_VERSION="547a3f28a0f53e77a8d8c8a0fa2e1be505ed2f0f"
PKG_SHA256=""
PKG_ARCH="aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC/swt6621s"
PKG_URL="https://github.com/CoreELEC/swt6621s/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="Seekwave SWT6621S SDIO Wi-Fi Linux driver"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

make_target() {
  # create version file or build can fail sometimes
  perl -s ${PKG_BUILD}/drivers/swt6621s_wifi/genver.pl \
    ${PKG_BUILD}/drivers/swt6621s_wifi/version.h

  kernel_make -C ${PKG_BUILD} \
    M=${PKG_BUILD} \
    KERNEL_SRC=$(kernel_path)
}

makeinstall_target() {
  mkdir -p ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
    find ${PKG_BUILD}/ -name \*.ko -not -path '*/\.*' -exec cp {} ${INSTALL}/$(get_full_module_dir)/${PKG_NAME} \;

  mkdir -p ${INSTALL}/$(get_full_firmware_dir)
    cp ${PKG_BUILD}/firmware/* ${INSTALL}/$(get_full_firmware_dir)
}
