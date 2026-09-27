# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="visualization.projectm"
PKG_VERSION="22.2.1-Piers"
PKG_SHA256="b40fa210a34b824b3309ea7aa85d79899862e6fbd8d3801c969b58b7d3c3a2c0"
PKG_REV="1"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/visualization.projectm"
PKG_URL="https://github.com/xbmc/visualization.projectm/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host libprojectM"
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
