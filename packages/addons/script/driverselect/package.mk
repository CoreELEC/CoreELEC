# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2020-present Team CoreELEC (https://coreelec.org)

PKG_NAME="driverselect"
PKG_VERSION="17d69cfa9781b9f987428cccd22a240bec4ebf9b"
PKG_SHA256="fd9e73b5618b52be9e95a7c2392ac49544a7d153e418c515dcff9915aaee7e2c"
PKG_REV="100"
PKG_ARCH="any"
PKG_LICENSE="OSS"
PKG_SITE="https://libreelec.tv"
PKG_URL="https://github.com/CoreELEC/script.program.driverselect/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="script.program"
PKG_SHORTDESC="script.program.driverselect"
PKG_LONGDESC="script.program.driverselect"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="embedded"
PKG_ADDON_NAME="Driver Select"
PKG_ADDON_TYPE="xbmc.python.script"

unpack() {
  mkdir -p ${PKG_BUILD}/addon
  tar --strip-components=1 -xf ${SOURCES}/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.gz -C ${PKG_BUILD}/addon
}

make_target() {
  :
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/share/kodi/addons/${PKG_SECTION}.${PKG_NAME}
  cp -rP ${PKG_BUILD}/addon/* ${INSTALL}/usr/share/kodi/addons/${PKG_SECTION}.${PKG_NAME}
}
