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
    PKG_VERSION="e2b2b6251eef8e758422f53c9e66fc78469180ad"
    PKG_SHA256="bd0c4caa2167c3cea7eb4313c57ef6305d5978ce3f35110803512e0ffc791b97"
    PKG_URL="https://github.com/CoreELEC/brcmfmac_sdio-firmware-aml/archive/$PKG_VERSION.tar.gz"
    ;;
  amlogic-4.9)
    PKG_VERSION="d3f239990c90edda89d3e32655ce091c9281820d"
    PKG_SHA256="1f0a4a69f71158c55ee5c258e392c790cf608e5dbaabe5287ef19728910b8124"
    PKG_URL="https://github.com/CoreELEC/brcmfmac_sdio-firmware-aml/archive/$PKG_VERSION.tar.gz"
    ;;
esac

makeinstall_target() {
  DESTDIR=$INSTALL FWDIR=$INSTALL/$(get_kernel_overlay_dir) make install

  if [ "$LINUX" = "amlogic-3.14" ]; then
    cd $INSTALL/$(get_full_firmware_dir)/brcm
    for f in *.hcd; do
      ln -sr $f $(grep --text -o 'BCM\S*' $f).hcd 2>/dev/null || true
      ln -sr $f $(grep --text -o 'BCM\S*' $f | cut -c4-).hcd 2>/dev/null || true
      ln -sr $f $(echo $f | sed -r 's/[^.]*/\U&/') 2>/dev/null || true
      ln -sr bcm4335_V0343.0353.hcd BCM4335A0.hcd 2>/dev/null || true
    done
  fi
}
