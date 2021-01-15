# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="visualization.matrix"
PKG_REV="3"
PKG_VERSION="v0.4.2"
PKG_SHA256="2fb66404c43d6279a403d9ef9a2bc18d0944bae2033bc1cd7b1f78b5892d5c0c"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/xbmc/visualization.matrix"
PKG_URL="https://github.com/xbmc/visualization.matrix/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain kodi-platform glm"
PKG_SECTION=""
PKG_LONGDESC="visualization.matrix"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.player.musicviz"

if [ ! "$OPENGL" = "no" ]; then
  # for OpenGL (GLX) support
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET $OPENGL glew"
fi

if [ "$OPENGLES_SUPPORT" = yes ]; then
  # for OpenGL-ES support
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET $OPENGLES"
fi
