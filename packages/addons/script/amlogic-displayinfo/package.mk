# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="amlogic-displayinfo"
PKG_VERSION="edf88414ce709ed7b1306ec85ddc5366bf24c848"
PKG_SHA256="b1f469f2797dd15eb7172ec4945ae6bed285a0b853d84f67679f57e9706647c4"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/roidy/script.amlogic.displayinfo"
PKG_URL="https://github.com/roidy/script.amlogic.displayinfo/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="script"
PKG_SHORTDESC="DisplayInfo: A script for showing display and video information during playback"
PKG_LONGDESC="DisplayInfo: A script for showing display and video information during playback"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.python.script"

unpack() {
  mkdir -p $PKG_BUILD/addon
  tar --strip-components=1 -xf $SOURCES/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.gz -C $PKG_BUILD/addon
}

make_target() {
  :
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/share/kodi/addons/${PKG_SECTION}.${PKG_NAME}
  cp -rP $PKG_BUILD/addon/* $INSTALL/usr/share/kodi/addons/${PKG_SECTION}.${PKG_NAME}
}
