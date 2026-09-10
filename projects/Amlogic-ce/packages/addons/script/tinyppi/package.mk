# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)

PKG_NAME="tinyppi"
PKG_VERSION="6b301753b934211bd793f52427d198c95fd5f629"
PKG_SHA256="bc9cf4d2b459a4244818ca74cb07894e92b9865be1dd0bc609bb012d084f218d"
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
PKG_ADDON_VERSION="2.8.1"

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp -PR ${PKG_BUILD}/* ${ADDON_BUILD}/${PKG_ADDON_ID}
}
