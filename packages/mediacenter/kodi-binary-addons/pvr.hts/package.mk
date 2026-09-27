# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.hts"
PKG_VERSION="22.9.2-piers"
PKG_SHA256="6aa7ad8e9579de69085f295207827fba6910ae38e9150d7114497344a585e28e"
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
