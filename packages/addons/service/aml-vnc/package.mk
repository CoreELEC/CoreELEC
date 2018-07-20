# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present CoreELEC (https://coreelec.org)

PKG_NAME="aml-vnc"
PKG_VERSION="0.1"
PKG_REV="3"
PKG_ARCH="arm aarch64"
PKG_LICENSE="OSS"
PKG_SITE="https://github.com/kszaq/aml-vnc"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain libvncserver"
PKG_PRIORITY="optional"
PKG_SECTION="service/system"
PKG_SHORTDESC="VNC Server for Amlogic devices"
PKG_LONGDESC="VNC Server for Amlogic devices"
PKG_DISCLAIMER="this is an unofficial addon. please don't ask for support in libreelec forum / irc channel"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.service"
PKG_ADDON_PROVIDES=""

PKG_AUTORECONF="no"

PKG_MAINTAINER="kszaq (kszaquitto at gmail.com)"

makeinstall_target() {
  : # nop
}

addon() {
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID/bin
  cp -P $PKG_BUILD/aml-vnc $ADDON_BUILD/$PKG_ADDON_ID/bin
  debug_strip $ADDON_BUILD/$PKG_ADDON_ID/
}
