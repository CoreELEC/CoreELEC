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
  "S905")
    PKG_VERSION="2986172ef237858fb95bd5e4fa41ee89a660153e"
    PKG_URL="https://github.com/CoreELEC/u-boot_firmware/archive/$PKG_VERSION.tar.gz"
    PKG_SHA256="f22f472f6cff1707789e055a4552b6bcb57671ae7d89044a66fdc4a5803f5a08"
    ;;
esac

makeinstall_target() {
  :
}
