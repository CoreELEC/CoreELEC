# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="audiodecoder.sacd"
PKG_VERSION="22.0.5-Piers"
PKG_SHA256="4bb2c8f5ba45ec655d889cb9fd703cdc116608325b070c1f8bb300224bf0aca2"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/audiodecoder.sacd"
PKG_URL="https://github.com/xbmc/audiodecoder.sacd/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host wavpack"
PKG_SECTION=""
PKG_SHORTDESC="audiodecoder.sacd"
PKG_LONGDESC="audiodecoder.sacd"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="kodi.audiodecoder"
