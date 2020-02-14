# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="rtkbt-firmware-aml"
PKG_VERSION="28865e18c4dde6efc5ef34a7bb77ad5e9b85ba1c"
PKG_SHA256="4621a45cb5a66d05f93fa5710386b74bd6cbcb4e931531f5e22fc4d05131f5ea"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/khadas/android_hardware_amlogic_bluetooth"
PKG_URL="https://github.com/khadas/android_hardware_amlogic_bluetooth/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain rtk_hciattach"
PKG_LONGDESC="Realtek BT Linux firmware"
PKG_TOOLCHAIN="manual"

unpack() {
  mkdir -p $PKG_BUILD
  tar --strip-components=5 -xf $SOURCES/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.gz -C $PKG_BUILD android_hardware_amlogic_bluetooth-$PKG_VERSION/realtek/rtkbt/Firmware/TV
}

makeinstall_target() {
  FWDIR=$INSTALL/$(get_full_firmware_dir)/rtlbt

  mkdir -p $FWDIR
    cp -a $PKG_BUILD/rtl8723bs_* $FWDIR
    cp -a $PKG_BUILD/rtl8822b_* $FWDIR
    cp -a $PKG_BUILD/rtl8822cs_* $FWDIR
}
