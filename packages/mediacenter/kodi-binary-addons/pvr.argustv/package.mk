# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.argustv"
PKG_VERSION="21.0.0-Omega"
PKG_SHA256="61aef981183c1e834b5a3d69fc02b7db5081b6996d7c0a1667d18eb02f655fd0"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/kodi-pvr/pvr.argustv"
PKG_URL="https://github.com/kodi-pvr/pvr.argustv/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain kodi-platform jsoncpp"
PKG_SECTION=""
PKG_SHORTDESC="pvr.argustv"
PKG_LONGDESC="pvr.argustv"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.pvrclient"

pre_configure_target() {
  CXXFLAGS+=" -Wno-narrowing"
}
