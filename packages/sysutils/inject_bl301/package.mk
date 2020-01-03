# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="inject_bl301"
PKG_VERSION="2313a5b8fc6fae7251e5f02b6161da12468fa035"
PKG_SHA256="47d59141dae79ccb332d9608498e521d33febac2eda3e5056dbc129122a6efc6"
PKG_LICENSE="proprietary"
PKG_SITE="https://coreelec.org"
PKG_URL="https://sources.coreelec.org/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Tool to inject bootloader blob BL301.bin on internal eMMC"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/usr/sbin
  mkdir -p $INSTALL/etc/inject_bl301
    install -m 0755 inject_bl301 $INSTALL/usr/sbin/inject_bl301
    install -m 0755 $PKG_DIR/scripts/checkbl301.sh $INSTALL/usr/sbin/checkbl301
    install -m 0644 $PKG_DIR/config/bl301.conf $INSTALL/etc/inject_bl301/bl301.conf
}
