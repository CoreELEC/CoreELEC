# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="qca9377-firmware-aml"
PKG_VERSION="d1c340302f986b06c8164b3064f1fb5722b90c6b"
PKG_SHA256="6b4014885b7b8477e9c037aceac22dbe18fe0e587cde69c00f0fd6eb44d92df0"
PKG_ARCH="arm aarch64"
PKG_LICENSE="Public Domain"
PKG_SITE="https://github.com/CoreELEC/qca-firmware"
PKG_URL="https://github.com/CoreELEC/qca-firmware/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="qca9377 Linux firmware"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_firmware_dir)
    cp -PR * $INSTALL/$(get_full_firmware_dir)
}
