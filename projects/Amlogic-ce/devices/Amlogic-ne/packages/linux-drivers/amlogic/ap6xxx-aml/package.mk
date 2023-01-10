# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ap6xxx-aml"
PKG_VERSION="a7de2ab70e40a674cf850a34479a3cca1cbfbed2"
PKG_SHA256="dba996b39c3adca3a4d4c5d5d27e5d554ef654f9278edb0ac28c3b2d06232a6b"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC/ap6xxx-aml"
PKG_URL="https://github.com/CoreELEC/ap6xxx-aml/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="ap6xxx: Linux drivers for AP6xxx WLAN chips used in some devices based on Amlogic SoCs"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

make_target() {
  echo
  echo "building ap6275s"
  kernel_make -C  ${PKG_BUILD}/bcmdhd.101.10.361.x \
       M=${PKG_BUILD}/bcmdhd.101.10.361.x \
       PWD=${PKG_BUILD}/bcmdhd.101.10.361.x \
       KERNEL_SRC=$(kernel_path) \
       CONFIG_BCMDHD_DISABLE_WOWLAN=y \
       CONFIG_BCMDHD_SDIO=y \
       bcmdhd_sdio

  echo
  echo "building ap6356s and others"
  kernel_make -C  ${PKG_BUILD}/bcmdhd.100.10.315.x \
       M=${PKG_BUILD}/bcmdhd.100.10.315.x \
       PWD=${PKG_BUILD}/bcmdhd.100.10.315.x \
       KERNEL_SRC=$(kernel_path) \
       CONFIG_BCMDHD_DISABLE_WOWLAN=y \
       CONFIG_BCMDHD_SDIO=y \
       bcmdhd_sdio
}

makeinstall_target() {
  mkdir -p ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
  cp ${PKG_BUILD}/*/*.ko ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
}
