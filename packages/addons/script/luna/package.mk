# SPDX-License-Identifier: GPL-3.0
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="luna"
PKG_VERSION="57c62904af9ec21bde2e49ebdb28524935e72090"
PKG_SHA256="c6f4aedc7c8b2ee7472ba4046e59a0bfd07d36ea7274082f0a31883e1d7e61e7"
PKG_REV="130"
PKG_ARCH="any"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/TheChoconut/Luna/"
PKG_URL="https://github.com/TheChoconut/Luna/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain moonlight-embedded"
PKG_SECTION="script"
PKG_SHORTDESC="Moonlight launcher for Kodi"
PKG_LONGDESC="Luna is a Moonlight Launcher for Kodi. Launch moonlight games directly from Kodi."
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="dummy"

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}
    cp -PR ${PKG_BUILD}/* ${ADDON_BUILD}/${PKG_ADDON_ID}

  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -P $(get_install_dir moonlight-embedded)/usr/bin/moonlight ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -P $(get_install_dir moonlight-embedded)/usr/etc/moonlight.conf ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -P $(get_install_dir moonlight-embedded)/usr/share/moonlight/gamecontrollerdb.txt ${ADDON_BUILD}/${PKG_ADDON_ID}/bin

  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/lib
    cp $(get_install_dir moonlight-embedded)/usr/lib/* ${ADDON_BUILD}/${PKG_ADDON_ID}/lib

}
