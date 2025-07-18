# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2025-present Team CoreELEC (https://coreelec.org)

PKG_NAME="firecube.toolbox"
PKG_VERSION="f92f069a15ac738c29f47cc569cbdc8427609e30"
PKG_SHA256="9315a0529cbe12551bc7c9dcc8efa8c1d7a31dc50af249bdccd107604823e258"
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
