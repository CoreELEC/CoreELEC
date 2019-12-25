# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="inject_bl301"
PKG_VERSION="5e76511d63d3e824f2b651cb40eaf5ee90f853fe"
PKG_SHA256="e2fd5e0c51900c77f312e534eb48fcf91993dd20939ac58e9388e03c198830cd"
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
