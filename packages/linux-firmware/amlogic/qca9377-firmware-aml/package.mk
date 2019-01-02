# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="qca9377-firmware-aml"
PKG_VERSION="5e4b71211ecbb79e7693d2ee07361847f5a0cb40"
PKG_SHA256="366bd14df08b4c31a653ce3e0d586854ba1e510dcab0487454e0d30bdc6ca56f"
PKG_ARCH="arm aarch64"
PKG_LICENSE="BSD-3c"
PKG_SITE="https://github.com/boundarydevices/qca-firmware"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="qca9377 Linux firmware"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_firmware_dir)
    cp -a * $INSTALL/$(get_full_firmware_dir)
}
