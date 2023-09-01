# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.vdr.vnsi"
PKG_VERSION="21.1.0-Omega"
PKG_SHA256="5c7317814d18439a36b6dc45e964aa13724ac19b46344d0d9196356f7c42921c"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/kodi-pvr/pvr.vdr.vnsi"
PKG_URL="https://github.com/kodi-pvr/pvr.vdr.vnsi/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain kodi-platform"
PKG_SECTION=""
PKG_SHORTDESC="pvr.vdr.vnsi"
PKG_LONGDESC="pvr.vdr.vnsi"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.pvrclient"

if [ "${OPENGLES_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGLES}"
fi

if [ "${OPENGL_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGL}"
fi
