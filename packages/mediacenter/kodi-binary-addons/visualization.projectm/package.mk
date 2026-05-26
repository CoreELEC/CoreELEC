# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="visualization.projectm"
PKG_VERSION="22.1.0-Piers"
PKG_SHA256="9c6b69eb8d62ae309bd1c02695a5182ece9f2f8504c59d4f17acb669685fc273"
PKG_REV="1"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/visualization.projectm"
PKG_URL="https://github.com/xbmc/visualization.projectm/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain kodi-platform libprojectM"
PKG_SECTION=""
PKG_SHORTDESC="visualization.projectm"
PKG_LONGDESC="visualization.projectm"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.player.musicviz"

if [ "${OPENGL_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGL}"
fi

pre_configure_target() {
  export LDFLAGS=$(echo ${LDFLAGS} | sed -e "s|-Wl,--as-needed||")
}
