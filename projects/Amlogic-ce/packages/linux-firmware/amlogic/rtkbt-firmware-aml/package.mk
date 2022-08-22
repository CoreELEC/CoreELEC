# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="rtkbt-firmware-aml"
PKG_VERSION="36f9ce681687223a8138c711e7a3d8271a4435db"
PKG_SHA256="506eaefab6413a18f1508734e8a33b3d31ad3cc50db77ddf71a435035229e22f"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/Stane1983/rtkbt-firmware-aml"
PKG_URL="https://github.com/Stane1983/rtkbt-firmware-aml/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain rtk_hciattach"
PKG_LONGDESC="Realtek BT Linux firmware"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  FWDIR=$INSTALL/$(get_full_firmware_dir)/rtlbt

  mkdir -p $FWDIR
    cp -a $PKG_BUILD/* $FWDIR
}
