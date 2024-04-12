# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ap6xxx-aml"
PKG_VERSION="b545efedb7876958056275746234d0a3c5047eaa"
PKG_SHA256="d7791ead27083a728d0ae70fb8cdba1ac8a56a81a1278215af87eb6813190888"
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
  kernel_make -C  ${PKG_BUILD}/bcmdhd.101.10.591.x \
       M=${PKG_BUILD}/bcmdhd.101.10.591.x \
       PWD=${PKG_BUILD}/bcmdhd.101.10.591.x \
       KERNEL_SRC=$(kernel_path) \
       CONFIG_BCMDHD_DISABLE_WOWLAN=y \
       CONFIG_BCMDHD_SDIO=y \
       bcmdhd_sdio

  echo "building ap6275p"
  kernel_make -C  ${PKG_BUILD}/bcmdhd.101.10.591.x \
       M=${PKG_BUILD}/bcmdhd.101.10.591.x \
       PWD=${PKG_BUILD}/bcmdhd.101.10.591.x \
       KERNEL_SRC=$(kernel_path) \
       CONFIG_BCMDHD_DISABLE_WOWLAN=y \
       CONFIG_BCMDHD_PCIE=y \
       bcmdhd_pcie

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
