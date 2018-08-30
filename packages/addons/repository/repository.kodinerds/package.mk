# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="repository.kodinerds"
PKG_VERSION="9.0"
PKG_REV="103"
PKG_ARCH="none"
PKG_LICENSE="GPL"
PKG_SITE="http://www.kodinerds.net"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION=""
PKG_SHORTDESC="Kodinerds add-on repository"
PKG_LONGDESC="Kodinerds add-on repository"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Kodinerds Repository"
PKG_ADDON_TYPE="xbmc.addon.repository"

make_target() {
  $SED -e "s|@PKG_VERSION@|$PKG_VERSION|g" \
       -e "s|@PKG_REV@|$PKG_REV|g" \
       -i addon.xml
}

addon() {
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID
  cp -R $PKG_BUILD/* $ADDON_BUILD/$PKG_ADDON_ID
}
