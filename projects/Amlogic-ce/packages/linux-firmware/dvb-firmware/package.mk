# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-2018 Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="dvb-firmware"
PKG_VERSION="1.4.0"
PKG_SHA256="8831e5e9c88e343742083f62bd28ad56b8cbd21bb9fe730ed1c292f6689fe2c3"
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
