# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2025-present Team CoreELEC (https://coreelec.org)

PKG_NAME="firecube.toolbox"
PKG_VERSION="e2bb70c1da1cf71c4309d205445678af54b436ae"
PKG_SHA256="f68a293ec5e630342e32791f5371d22f4b2a51eeb807638876fa31adb7ec457c"
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
