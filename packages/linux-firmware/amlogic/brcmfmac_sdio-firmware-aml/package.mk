# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="brcmfmac_sdio-firmware-aml"
PKG_VERSION="15e79c977a136efc0bae87a2190e5bc6f7e524a9"
PKG_SHA256="867f30fb0f3a9ce10cb047663aeaf8a322d39f391a6979db40ee97070fcd18ad"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC/brcmfmac_sdio-firmware-aml"
PKG_URL="https://github.com/CoreELEC/brcmfmac_sdio-firmware-aml/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Firmware for brcm bluetooth chips used in some Amlogic based devices."

makeinstall_target() {
  DESTDIR=$INSTALL FWDIR=$INSTALL/$(get_kernel_overlay_dir) make install
}
