# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.hts"
PKG_VERSION="22.9.4-Piers"
PKG_SHA256="1d05cbbd0395bfdb348bd601128d78f656289a1675aa51b5d4d204d19b4e067f"
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
