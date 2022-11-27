# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="uwe5631-aml"
PKG_VERSION="7e756e49720943e6f0d15821b2f41b73309a1430"
PKG_SHA256="4e41ae5cc427ba02a7ba53bdd3fdf7a6908f6d1314f2f764c0947ac82908d750"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC/uwe5631-aml"
PKG_URL="https://github.com/CoreELEC/uwe5631-aml/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="uwe5631-aml: Unisoc UWE5621 WIFI/BT driver"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

make_target() {
  echo "making WIFI"
  kernel_make -C ${PKG_BUILD} \
    M=${PKG_BUILD} \
    KERNEL_SRC=$(kernel_path) \
    EXTRA_CFLAGS="-fno-pic -Wno-sizeof-pointer-memaccess -Wno-declaration-after-statement -I${PKG_BUILD}/BSP/include -DCUSTOMIZE_WIFI_CFG_PATH=\\\"/lib/firmware/unisoc\\\"" \
    modules

  echo "making BT"
  kernel_make -C ${PKG_BUILD}/BT/tty-sdio \
    M=${PKG_BUILD}/BT/tty-sdio \
    KERNEL_SRC=$(kernel_path) \
    CURFOLDER=${PKG_BUILD}/BSP \
    modules
}

makeinstall_target() {
  mkdir -p ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}

  find $PKG_BUILD/ -name \*.ko -not -path '*/\.*' \
    -exec cp {} ${INSTALL}/$(get_full_module_dir)/${PKG_NAME} \;

  mkdir -p ${INSTALL}/$(get_kernel_overlay_dir)/lib/firmware/unisoc

  cp -av ${PKG_DIR}/firmware/*.ini \
         ${PKG_BUILD}/BT/libbt/conf/sprd/runtime/*.ini \
         ${PKG_BUILD}/BSP/fw/wcnmodem.bin \
    ${INSTALL}/$(get_kernel_overlay_dir)/lib/firmware/unisoc
}
