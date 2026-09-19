# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)

PKG_NAME="tinyppi"
PKG_VERSION="d9804618f398bba35ecb64edba931e137f958e2e"
PKG_SHA256="4c5fb52d475cf09e3e848ab3951132e4b0ae76502d3ddb36bb276e994d283f28"
PKG_ARCH="aarch64"
PKG_LICENSE="AGPL-3.0-or-later"
PKG_SITE="https://github.com/CE-Repo/script.tinyppi"
PKG_URL="https://github.com/CE-Repo/script.tinyppi/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="script"
PKG_SHORTDESC="Displays custom player process information"
PKG_LONGDESC="Opens a separate process information window. This window displays codec, video, and audio details for the currently playing media."
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="TinyPPI"
PKG_ADDON_TYPE="xbmc.python.script"
PKG_ADDON_VERSION="2.9.6"

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp -PR ${PKG_BUILD}/* ${ADDON_BUILD}/${PKG_ADDON_ID}
}
