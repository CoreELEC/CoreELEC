# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="amlogic-boot-fip"
PKG_LICENSE="nonfree"
PKG_VERSION="42d372123631066fb77fbcbb612dc3eb41a3f6f9"
PKG_SHA256="dede20dd7666a33b33442252b994f4033cfafe3fb6d11c86019dc3ae61d55227"
PKG_SITE="https://github.com/LibreELEC/amlogic-boot-fip"
PKG_URL="https://github.com/LibreELEC/amlogic-boot-fip/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Firmware Image Package (FIP) sources used to sign Amlogic u-boot binaries in LibreELEC images"
PKG_TOOLCHAIN="manual"
PKG_STAMP="${UBOOT_SYSTEM}"

post_unpack() {
  # rename dirs for alta/solitude
  mv ${PKG_BUILD}/aml-a311d-cc ${PKG_BUILD}/alta
  mv ${PKG_BUILD}/aml-s905d3-cc ${PKG_BUILD}/solitude
}
