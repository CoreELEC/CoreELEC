# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="amlogic.displayinfo"
PKG_VERSION="0.1"
PKG_REV="100"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/roidy/script.amlogic.displayinfo"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="script"
PKG_SHORTDESC="A script for showing display and video information during playback"
PKG_LONGDESC="A script for showing display and video information during playback in the player process info window (KEY_O)"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="embedded"
PKG_ADDON_NAME="CoreELEC Display Info"
PKG_ADDON_PROJECTS="Amlogic Amlogic-ng"
PKG_ADDON_TYPE="xbmc.python.script"
PKG_MAINTAINER="roidy"

makeinstall_target() {
  install_addon_files "${INSTALL}/usr/share/kodi/addons/${PKG_SECTION}.${PKG_NAME}"
}

addon() {
  :
}
