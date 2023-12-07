# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="script.config.vdr"
PKG_VERSION="3635c3751a4fe84e54a432b0efe6a969f74793e4"
PKG_SHA256="f87fa7de4c7b9917da0ed2a383fd3d49cee1b88bdab0f03b5c1e14b15fb3ad76"
PKG_REV="0"
PKG_ARCH="any"
PKG_LICENSE="OSS"
PKG_SITE="https://libreelec.tv"
PKG_URL="https://github.com/LibreELEC/script.config.vdr/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="xmlstarlet:host 7-zip:host"
PKG_SECTION=""
PKG_SHORTDESC="script.config.vdr"
PKG_LONGDESC="script.config.vdr"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="dummy"

make_target() {
  sed -e "s|@ADDON_VERSION@|${ADDON_VERSION}|g" \
      -i addon.xml
}

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp -PR ${PKG_BUILD}/* ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp ${PKG_DIR}/changelog.txt ${ADDON_BUILD}/${PKG_ADDON_ID}
  cp ${PKG_DIR}/icon/icon.png ${ADDON_BUILD}/${PKG_ADDON_ID}/resources
}
