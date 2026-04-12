# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="p8-platform"
PKG_VERSION="p8-platform-20260129"
PKG_SHA256="216b10c8d8f6d99c34e89be8a400449ad205b57f40b24e750b804922ed5afba1"
PKG_LICENSE="GPL"
PKG_SITE="http://www.kodi.tv"
PKG_URL="https://github.com/xbmc/platform/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Platform support library used by libCEC and binary add-ons for Kodi"

PKG_CMAKE_OPTS_TARGET="-DCMAKE_INSTALL_LIBDIR:STRING=lib \
                       -DBUILD_SHARED_LIBS=0"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr
}
