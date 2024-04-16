# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="inputstream.adaptive"
PKG_VERSION="21.4.5-Omega"
PKG_SHA256="6e7d217b239f7cb935fd4f4024353ba995fcdb47078a35e0ae51ee5dcf8c8de8"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/xbmc/inputstream.adaptive"
PKG_URL="https://github.com/xbmc/inputstream.adaptive/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain kodi-platform bento4 nss pugixml rapidjson"
PKG_SECTION=""
PKG_SHORTDESC="inputstream.adaptive"
PKG_LONGDESC="inputstream.adaptive"

PKG_IS_ADDON="yes"

addon() {
  install_binary_addon ${PKG_ADDON_ID}

  if [ "${ARCH}" = "aarch64" ]; then
    mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/lib/cdm_aarch64/libcdm_aarch64_loader.so ${ADDON_BUILD}/${PKG_ADDON_ID}
  fi
}
