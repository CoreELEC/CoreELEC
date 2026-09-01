# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="imagedecoder.mpo"
PKG_VERSION="22.1.0-Piers"
PKG_SHA256="e518db490f1573fb1ded92ba82ac88a9663ce0b409bf869c534ec1b7e1c721ea"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/imagedecoder.mpo"
PKG_URL="https://github.com/xbmc/imagedecoder.mpo/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host libjpeg-turbo tinyxml2"
PKG_SECTION=""
PKG_SHORTDESC="imagedecoder.mpo"
PKG_LONGDESC="imagedecoder.mpo"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="kodi.imagedecoder"
