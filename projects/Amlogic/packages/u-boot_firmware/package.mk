# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present CoreELEC (https://coreelec.org)

PKG_NAME="u-boot_firmware"
PKG_SITE="https://github.com/hardkernel/u-boot_firmware/"
PKG_DEPENDS_TARGET="toolchain"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SECTION="tools"
PKG_SHORTDESC="u-boot_firmware: Required firmware Files for U-Boot"
PKG_LONGDESC=""
PKG_TOOLCHAIN=manual

case "$DEVICE" in
  "Odroid_C2")
    PKG_VERSION="b7b90c1099b057d35ebae886b7846b5d9bfb4143"
    PKG_URL="https://github.com/hardkernel/u-boot_firmware/archive/$PKG_VERSION.tar.gz"
    PKG_SHA256="39bf7c7a62647699572e088259cfe514579c09fa1b1b1ab3fade857b27da5ce9"
    ;;
esac

makeinstall_target() {
  :
}
