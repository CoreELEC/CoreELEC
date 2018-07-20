# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018 Arthur Liberman (arthur_liberman@hotmail.com)

PKG_NAME="service.openvfd"
PKG_VERSION="1.0.1"
PKG_SHA256="05d498bce3214252f82744ef21ae7f7c49b96dc47bfccbe4a7d436c26f59ea90"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/arthur-liberman/service.openvfd"
PKG_URL="https://github.com/arthur-liberman/service.openvfd/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain kodi-platform"
PKG_SECTION=""
PKG_SHORTDESC="service.openvfd"
PKG_LONGDESC="service.openvfd"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.service"

addon() {
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID
  cp -PR $PKG_BUILD/* $ADDON_BUILD/$PKG_ADDON_ID
}
