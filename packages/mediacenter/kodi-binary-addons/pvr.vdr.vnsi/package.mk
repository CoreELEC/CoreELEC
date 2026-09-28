# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.vdr.vnsi"
PKG_VERSION="22.3.2-Piers"
PKG_SHA256="d26082c4c19b37f3c09bf3e363d89074981822b611c5a5436124652dc7fcc395"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/kodi-pvr/pvr.vdr.vnsi"
PKG_URL="https://github.com/kodi-pvr/pvr.vdr.vnsi/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host"
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
