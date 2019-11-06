# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_VERSION="f905079f320da9d0d95d3601b4fc0f5a5e7217bb"
PKG_SHA256="d39ba4c8bc178ffdcf0bf9424fec12fff0c25552a2031c4c5f3b2dc7b592a7b0"
PKG_LICENSE="proprietary"
PKG_SITE="https://coreelec.org"
PKG_URL="https://sources.coreelec.org/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Tool to install CoreELEC on internal eMMC"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/usr/sbin
    install -m 0755 ceemmc $INSTALL/usr/sbin/ceemmc
}
