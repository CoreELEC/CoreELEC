# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="audioencoder.flac"
PKG_VERSION="22.0.3-Piers"
PKG_SHA256="11003add10d18d4d397ed3f35812f8ffd2f42075b802c0dcc1b1a56975467695"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/audioencoder.flac"
PKG_URL="https://github.com/xbmc/audioencoder.flac/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host flac"
PKG_SECTION=""
PKG_SHORTDESC="audioencoder.flac: A audioencoder addon for Kodi"
PKG_LONGDESC="audioencoder.flac is a audioencoder addon for Kodi"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.audioencoder"
