# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="kodi-platform"
PKG_VERSION="844cf3ef5ad0649b8d97894d422928ec8a7a7d71"
PKG_SHA256="3215ff51649c852645e499917b69c7393f3998cd458440ab6025c1ce2434dd88"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="http://www.kodi.tv"
PKG_URL="https://github.com/xbmc/kodi-platform/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host p8-platform"
PKG_LONGDESC="kodi-platform:"

PKG_CMAKE_OPTS_TARGET="-DCMAKE_INSTALL_LIBDIR:STRING=lib \
                       -DCMAKE_INSTALL_LIBDIR_NOARCH:STRING=lib \
                       -DBUILD_SHARED_LIBS=OFF"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/lib/kodiplatform
}
