# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)

PKG_NAME="sidedata"
PKG_VERSION="1.6.0"
PKG_SHA256="e479f2b21f5ab68f715af48581ffbe0480d49d2732d1e572ce8b3c5c5abd0d24"
PKG_REV="0"
PKG_ARCH="aarch64"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/matthane/script.module.sidedata"
PKG_URL="https://github.com/matthane/script.module.sidedata/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="script.module"
PKG_SHORTDESC="Per frame video metadata for CoreELEC 22"
PKG_LONGDESC="Reads the raw video metadata CoreELEC 22 publishes frame by frame during playback. Addons get it as a plain dict, skins get it as Home window properties."
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Sidedata"
PKG_ADDON_TYPE="xbmc.python.module"

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp -PR ${PKG_BUILD}/* ${ADDON_BUILD}/${PKG_ADDON_ID}
}
