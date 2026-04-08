# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="filebrowser"
PKG_VERSION="2.63.1"
PKG_REV="3"
PKG_LICENSE="Apache License 2.0"
PKG_SITE="https://filebrowser.org"
PKG_DEPENDS_TARGET="toolchain:host"
PKG_SECTION="service"
PKG_SHORTDESC="Filebrowser: a web based filemanger"
PKG_LONGDESC="Filebrowser (${PKG_VERSION}): is a web based file managing interface and it can be used to upload, delete, preview, rename and edit your files."
PKG_TAR_STRIP_COMPONENTS="no"
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="5fda7ad30c54df9ac28bb11a375523a7ed5f11ddaed4001a4f38f9530bf3bde3"
    PKG_URL="https://github.com/filebrowser/filebrowser/releases/download/v${PKG_VERSION}/linux-arm64-filebrowser.tar.gz"
    ;;
  "arm")
    PKG_SHA256="12d4c38b4ca4f2bb4401187b5be89666c50cbda4edc0440553256dd99d7ee2e3"
    PKG_URL="https://github.com/filebrowser/filebrowser/releases/download/v${PKG_VERSION}/linux-armv7-filebrowser.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="298a491ae48ae929a2bf10f6fe6f4a63057f8535810244414f078b4e336e859b"
    PKG_URL="https://github.com/filebrowser/filebrowser/releases/download/v${PKG_VERSION}/linux-amd64-filebrowser.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="filebrowser-${PKG_VERSION}-${ARCH}.tar.gz"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Web File Browser"
PKG_ADDON_PROJECTS="any !RPi1"
PKG_ADDON_TYPE="xbmc.service"

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
  cp -r ${PKG_BUILD}/filebrowser ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
}
