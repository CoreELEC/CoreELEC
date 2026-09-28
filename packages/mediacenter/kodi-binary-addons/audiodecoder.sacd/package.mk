# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="audiodecoder.sacd"
PKG_VERSION="22.0.4-Piers"
PKG_SHA256="f422fe2d07a6f7044fd38d982c1b4ef4c8a268a06afd39dc3690cdce96bb58e1"
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
