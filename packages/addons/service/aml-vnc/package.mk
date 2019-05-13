# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="aml-vnc"
PKG_VERSION="e3e0a43a47d362f3acc1aaafc966d81506f66d11"
PKG_SHA256="d3c556e91b207ea49c1e384d3d03bcd6555d77e4b8014cc6037469b90b08df88"
PKG_REV="101"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/kszaq/aml-vnc"
PKG_URL="https://github.com/kszaq/my-addons/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain libvncserver"
PKG_SECTION="service"
PKG_SHORTDESC="Amlogic VNC server"
PKG_LONGDESC="Amlogic VNC server is a Virtual Network Computing (VNC) server for Amlogic devices"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Amlogic VNC"
PKG_ADDON_TYPE="xbmc.service"

unpack() {
  mkdir -p $PKG_BUILD/addon
  tar --strip-components=3 -xf $SOURCES/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.gz -C $PKG_BUILD my-addons-$PKG_VERSION/$PKG_NAME/sources
  tar --strip-components=3 -xf $SOURCES/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.gz -C $PKG_BUILD/addon my-addons-$PKG_VERSION/$PKG_NAME/source
}

makeinstall_target() {
  :
}

addon() {
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID/bin
    cp -P $PKG_BUILD/aml-vnc $ADDON_BUILD/$PKG_ADDON_ID/bin

  cp -PR $PKG_BUILD/addon/* $ADDON_BUILD/$PKG_ADDON_ID
}
