# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="amremote"
PKG_ARCH="arm aarch64"
PKG_LICENSE="other"
PKG_SITE="http://www.amlogic.com"
PKG_DEPENDS_TARGET="toolchain usbutils"
PKG_LONGDESC="amremote - IR remote configuration utility for Amlogic-based devices"

case "$LINUX" in
  amlogic-3.14)
    PKG_VERSION="3693417803e660796043ea7443a1621ad146db38"
    PKG_SHA256="8c7aadbca4d01b6f4f8997935b624b26147115c3c2dd421edcd46937ad1b6892"
    PKG_URL="https://github.com/CoreELEC/amremote/archive/$PKG_VERSION.tar.gz"
    ;;
  amlogic-4.9)
    PKG_VERSION="1db130a0ccd47f6b5c3d1dffab1e89613b796a8c"
    PKG_SHA256="5b96f2a1dd03200909eed749f5d97d1d02ee7fc8ac92d8fce6b5d6772ee642dc"
    PKG_URL="https://github.com/CoreELEC/amremote/archive/$PKG_VERSION.tar.gz"
    ;;
esac

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
    cp remotecfg $INSTALL/usr/bin

  mkdir -p $INSTALL/usr/lib/coreelec
    cp $PKG_DIR/scripts/* $INSTALL/usr/lib/coreelec
}

post_install() {
  enable_service amlogic-remotecfg.service
  enable_service amlogic-remote-toggle.service
}
