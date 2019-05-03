# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="amlogic-displayinfo-addon"
PKG_VERSION="1.0"
PKG_LICENSE="MIT"
PKG_SITE=""
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A script for showing display and video information during playback"
PKG_TOOLCHAIN="manual"

make_target() {
(
  cd $ROOT
  scripts/create_addon script.amlogic.displayinfo
)
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/share/kodi/addons
    cp -R $BUILD/$ADDONS/script.amlogic.displayinfo/script.amlogic.displayinfo $INSTALL/usr/share/kodi/addons
}
