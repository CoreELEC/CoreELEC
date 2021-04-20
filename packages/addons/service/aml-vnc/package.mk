# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="aml-vnc"
PKG_VERSION="f87a8f41aa6c6463a99ec3be9aebaff3cf31c5ee"
PKG_SHA256="dbec8fbedc956b8730414654da4f90b035c1cb67e2320e5c572b1343fb029e36"
PKG_REV="103"
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
