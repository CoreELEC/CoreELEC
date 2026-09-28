# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.hdhomerun"
PKG_VERSION="22.2.6-Piers"
PKG_SHA256="ba2e3054a73e10697b7a9a31a205ce0ecabc0bbd47b69eb2878ed8d8884b3d9d"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/kodi-pvr/pvr.hdhomerun"
PKG_URL="https://github.com/kodi-pvr/pvr.hdhomerun/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host jsoncpp libhdhomerun"
PKG_SECTION=""
PKG_SHORTDESC="pvr.hdhomerun"
PKG_LONGDESC="pvr.hdhomerun"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.pvrclient"
