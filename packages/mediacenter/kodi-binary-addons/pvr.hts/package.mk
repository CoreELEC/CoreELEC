# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.hts"
PKG_VERSION="22.9.1-Piers"
PKG_SHA256="c45fec57aa26e4a57ceee8d7db2d1ec68c4f872d179e4e83419ca3add4aa54c0"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/kodi-pvr/pvr.hts"
PKG_URL="https://github.com/kodi-pvr/pvr.hts/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host"
PKG_SECTION=""
PKG_SHORTDESC="pvr.hts"
PKG_LONGDESC="pvr.hts"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.pvrclient"
