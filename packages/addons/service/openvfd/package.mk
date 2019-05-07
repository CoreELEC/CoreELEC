# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Arthur Liberman (arthur_liberman@hotmail.com)

PKG_NAME="openvfd"
PKG_VERSION="b8096fc8f2e8dc7e0005da6026c9382cc6135444"
PKG_SHA256="ea96288f9fb68f8c3b1b0b746892e81f9a2f1746a5bde54ea9af57c954a7fb6e"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/arthur-liberman/service.openvfd"
PKG_URL="https://github.com/arthur-liberman/service.openvfd/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="service"
PKG_SHORTDESC="OpenVFD: Service for controlling VFD displays"
PKG_LONGDESC="OpenVFD: Service for controlling VFD displays, e.g. Display icons (Ethernet/WiFi connection status) Time, Date, Playback time"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.service"

addon() {
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID
    cp -PR $PKG_BUILD/* $ADDON_BUILD/$PKG_ADDON_ID
}
