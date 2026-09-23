# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="imagedecoder.heif"
PKG_VERSION="22.1.1-Piers"
PKG_SHA256="390e27498dca00b058a99452dd4bf2d7f63023b4c78a6dd4210b539becd69db6"
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
