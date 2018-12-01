# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2011-present AlexELEC (http://www.alexelec.in.ua)

PKG_NAME="amremote"
PKG_VERSION="6431040"
PKG_SHA256="5859680b0951ed3d2265999b7ad5309060587815df4dd1c48c6fa9aae039d5c5"
PKG_ARCH="arm aarch64"
PKG_LICENSE="other"
PKG_SITE="http://www.amlogic.com"
PKG_URL="https://github.com/codesnake/amremote/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain usbutils"
PKG_SECTION="sysutils/remote"
PKG_SHORTDESC="amremote - IR remote configuration utility for Amlogic-based devices"
PKG_LONGDESC="amremote - IR remote configuration utility for Amlogic-based devices"

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
    cp remotecfg $INSTALL/usr/bin

  mkdir -p $INSTALL/usr/config/amremote
    cp $PKG_DIR/config/* $INSTALL/usr/config/amremote
}
