# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.stalker"
PKG_VERSION="22.2.6-Piers"
PKG_SHA256="6c11fb9b1b0304eab99c3f505b46c198f07fe1dde49d3c720b7932ab1467ced1"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/kodi-pvr/pvr.stalker"
PKG_URL="https://github.com/kodi-pvr/pvr.stalker/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host jsoncpp libxml2 tinyxml"
PKG_SECTION=""
PKG_SHORTDESC="pvr.stalker"
PKG_LONGDESC="pvr.stalker"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.pvrclient"
