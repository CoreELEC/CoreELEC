# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="imagedecoder.heif"
PKG_VERSION="22.1.0-Piers"
PKG_SHA256="ca62ee5b550af7cf24869ad28dc923ef72c515641f18d10f244b0e2205c27e5b"
PKG_REV="2"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/imagedecoder.heif"
PKG_URL="https://github.com/xbmc/imagedecoder.heif/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host libheif tinyxml2"
PKG_SECTION=""
PKG_SHORTDESC="imagedecoder.heif"
PKG_LONGDESC="imagedecoder.heif"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="kodi.imagedecoder"
