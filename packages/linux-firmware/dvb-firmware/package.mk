# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dvb-firmware"
PKG_VERSION="1.2.4"
PKG_SHA256="438171de4c28705651427e239a0b22f5d88292c14e7a34e389c001d00504ecad"
PKG_ARCH="any"
PKG_LICENSE="Free-to-use"
PKG_SITE="https://github.com/CoreELEC/dvb-firmware"
PKG_URL="https://github.com/CoreELEC/dvb-firmware/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="firmware"
PKG_SHORTDESC="dvb-firmware: firmwares for various DVB drivers"
PKG_LONGDESC="dvb-firmware: firmwares for various DVB drivers"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  DESTDIR=$INSTALL/$(get_kernel_overlay_dir) ./install
}
