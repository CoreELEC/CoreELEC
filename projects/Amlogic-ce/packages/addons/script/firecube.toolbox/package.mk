# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2025-present Team CoreELEC (https://coreelec.org)

PKG_NAME="firecube.toolbox"
PKG_VERSION="53448a368a269a8179a168073376536537305d3f"
PKG_SHA256="ae5ae859c2c3bdad81b7bb8ce88497d9f9661519f5e211d5c63a156000e88114"
PKG_REV="1"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/Pro-me3us/script.firecube.toolbox"
PKG_URL="https://github.com/Pro-me3us/script.firecube.toolbox/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET=""
PKG_SECTION="script"
PKG_SHORTDESC="Fire Cube Toolbox"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Fire Cube Toolbox"
PKG_ADDON_TYPE="xbmc.script"

addon() {
  mkdir -p "${ADDON_BUILD}/${PKG_ADDON_ID}"

  cp -P ${PKG_BUILD}/addon.xml ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp -P ${PKG_BUILD}/LICENSE.txt ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp -P ${PKG_BUILD}/README.md ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp -P ${PKG_BUILD}/*.py ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp -PR ${PKG_BUILD}/resources ${ADDON_BUILD}/${PKG_ADDON_ID}
}

post_install_addon() {
  cp ${PKG_BUILD}/resources/fanart.png ${ADDON_BUILD}/${PKG_ADDON_ID}/resources
}
