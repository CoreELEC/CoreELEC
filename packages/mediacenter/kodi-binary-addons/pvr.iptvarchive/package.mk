# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="pvr.iptvarchive"
PKG_VERSION="73feb2f4a3106cb3cf7f3f5432438ad1af2590a3"
PKG_SHA256="332cd69cf8991d8e789125b294fe877c3d742d8ce83afb189f366461c67b5a86"
PKG_REV="2"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://www.kodi.tv"
PKG_URL="https://github.com/kodi-pvr/pvr.iptvsimple/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain kodi-platform rapidxml zlib"
PKG_SECTION=""
PKG_SHORTDESC="pvr.iptvarchive"
PKG_LONGDESC="pvr.iptvarchive"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.pvrclient"

PKG_CMAKE_OPTS_TARGET="-DRAPIDXML_INCLUDE_DIRS=$(get_build_dir rapidxml)"

post_patch() {
	[ -d "${PKG_BUILD}/pvr.iptvsimple" ] && mv "${PKG_BUILD}/pvr.iptvsimple" "${PKG_BUILD}/pvr.iptvarchive"
}
