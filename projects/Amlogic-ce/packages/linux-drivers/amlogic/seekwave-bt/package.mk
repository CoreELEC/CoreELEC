# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)

PKG_NAME="seekwave-bt"
PKG_VERSION="bc51dce4cca1e498a1fff24f1db5e363055af793"
PKG_SHA256=""
PKG_ARCH="aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC/seekwave-bt"
PKG_URL="https://github.com/CoreELEC/seekwave-bt/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="Seekwave bluetooth HCI driver for the EA6X21QX and SWT6621S platforms"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

make_target() {
  kernel_make -C ${PKG_BUILD} KERNEL_SRC=$(kernel_path)
}

makeinstall_target() {
  mkdir -p ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
    cp ${PKG_BUILD}/swtbt4l/skwbt.ko ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}

  mkdir -p ${INSTALL}/$(get_full_firmware_dir)
    cp ${PKG_BUILD}/firmware/*.nvbin ${INSTALL}/$(get_full_firmware_dir)
}
