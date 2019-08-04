# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="brcmfmac_sdio-firmware-all-aml"
PKG_VERSION="2aff7faf90d71c0b3cdc8f47495a8503f4ccecfb"
PKG_SHA256="405062114c6e3df8f2aa8334702623f1b1720a6d5adbb98c39f41a9329c94e02"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/coreelec/brcmfmac_sdio-firmware-all-aml"
PKG_URL="https://github.com/coreelec/brcmfmac_sdio-firmware-all-aml/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Firmware for brcm bluetooth chips used in some Amlogic based devices."

post_makeinstall_target() {
  cd $INSTALL/$(get_full_firmware_dir)/brcm
  for f in *.hcd; do
    ln -sr $f $(grep --text -o 'BCM\S*' $f).hcd 2>/dev/null || true
    ln -sr $f $(grep --text -o 'BCM\S*' $f | cut -c4-).hcd 2>/dev/null || true
    ln -sr $f $(echo $f | sed -r 's/[^.]*/\U&/') 2>/dev/null || true
    ln -sr bcm4335_V0343.0353.hcd BCM4335A0.hcd 2>/dev/null || true
  done
}
