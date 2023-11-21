# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="gpu-aml"
PKG_VERSION="03aa93d12317abfb604b6f0ee9ffff9573d5539d"
PKG_SHA256="b91b95985fd02d24a3084c314b9386748f6e3fc2aee1a3b583934703cd2d713a"
PKG_LICENSE="GPL"
PKG_SITE="https://coreelec.org"
PKG_URL="https://github.com/CoreELEC/gpu-aml/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="gpu-aml: Linux drivers for Mali GPUs found in Amlogic Meson SoCs"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

make_target() {
  kernel_make -C $(kernel_path) M=${PKG_BUILD}/bifrost/r37p0/kernel/drivers/gpu/arm \
    CONFIG_MALI_MIDGARD=m CONFIG_MALI_PLATFORM_NAME="devicetree" \
    EXTRA_CFLAGS="-I${PKG_BUILD}/dvalin/kernel/include -DCONFIG_MALI_LOW_MEM=0"
}

makeinstall_target() {
  kernel_make -C $(kernel_path) M=${PKG_BUILD}/bifrost/r37p0/kernel/drivers/gpu/arm \
    INSTALL_MOD_PATH=${INSTALL}/$(get_kernel_overlay_dir) INSTALL_MOD_STRIP=1 DEPMOD=: \
  modules_install
}
