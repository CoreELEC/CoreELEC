# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2025-present Team CoreELEC (https://coreelec.org)

PKG_NAME="firecube.toolbox"
PKG_VERSION="aaa1d0a07be7fae1dfe322c8ca78c678f34ed157"
PKG_SHA256="06eea3303c99d2da79d7a7c6dc68520c5d863ac794d0459171e43a07b4a46785"
PKG_REV="0"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/Pro-me3us/script.firecube.toolbox"
PKG_URL="https://github.com/Pro-me3us/script.firecube.toolbox/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
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
