# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="brcmfmac_sdio-firmware-aml"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC/brcmfmac_sdio-firmware-aml"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Firmware for brcm bluetooth chips used in some Amlogic based devices."

case "$LINUX" in
  amlogic-3.14)
    PKG_VERSION="072bc005efcf9b3052ff93eb9596c27ca7983afe"
    PKG_SHA256="27fcfe7d18c2055305d668522bd2981152fd1a2227dab63dfffa6b70ade4b32a"
    PKG_URL="https://github.com/CoreELEC/brcmfmac_sdio-firmware-aml/archive/$PKG_VERSION.tar.gz"
    ;;
  amlogic-4.9)
    PKG_VERSION="1d94b5f006b7139bc00ff97314a2b25b358d36c7"
    PKG_SHA256="206a0a1cf33db607c4a441ce2e032a5dd2600412feee586efbbce3b2176b655a"
    PKG_URL="https://github.com/CoreELEC/brcmfmac_sdio-firmware-aml/archive/$PKG_VERSION.tar.gz"
    ;;
esac

makeinstall_target() {
  DESTDIR=$INSTALL FWDIR=$INSTALL/$(get_kernel_overlay_dir) make install

  if [ "$LINUX" = "amlogic-3.14" ]; then
    cd $INSTALL/$(get_full_firmware_dir)/brcm
    for f in *.hcd; do
      ln -sr $f $(grep --text -o 'BCM[24]\S*' $f).hcd 2>/dev/null || true
      ln -sr $f $(grep --text -o 'BCM[24]\S*' $f | cut -c4-).hcd 2>/dev/null || true
      ln -sr $f $(echo $f | sed -r 's/[^.]*/\U&/') 2>/dev/null || true
    done
    ln -sr bcm4335_V0343.0353.hcd bcm4335a0.hcd 2>/dev/null || true
  fi
}
