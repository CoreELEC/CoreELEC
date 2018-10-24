# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dvb-firmware"
PKG_VERSION="1.3.1"
PKG_SHA256="bb286f9d53ea36e13e51e68612d35f172d9625d4d8129c872bb3c2f9e5fc1bed"
PKG_LICENSE="Free-to-use"
PKG_SITE="https://github.com/CoreELEC/dvb-firmware"
PKG_URL="https://github.com/CoreELEC/dvb-firmware/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="dvb-firmware: firmwares for various DVB drivers"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  PKG_FW_DIR="$INSTALL/$(get_kernel_overlay_dir)/lib/firmware"
  mkdir -p "$PKG_FW_DIR"
    cp -a "$PKG_BUILD/firmware/"* "$PKG_FW_DIR"
}
