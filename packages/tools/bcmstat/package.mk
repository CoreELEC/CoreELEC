# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bcmstat"
PKG_VERSION="0.5.1"
PKG_SHA256="88b3b189170e7d0eee83a233565d9ef0fb323489dc1161ef2392fca50e9637d6"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/MilhouseVH/bcmstat"
PKG_URL="https://github.com/MilhouseVH/$PKG_NAME/archive/$PKG_VERSION.tar.gz"
PKG_LONGDESC="Raspberry Pi monitoring script"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
    cp -PRv bcmstat.sh $INSTALL/usr/bin
    chmod +x $INSTALL/usr/bin/*
}
