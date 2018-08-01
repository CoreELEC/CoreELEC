# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018 Arthur Liberman (arthur_liberman@hotmail.com)

PKG_NAME="service.openvfd"
PKG_VERSION="1.0.2"
PKG_SHA256="0dfbd8fc01169c2b2ddb9cf4d4bbd51b83c7074a1df876aa2b27b1e4d4dd9c9f"
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
