# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.iptvsimple"
PKG_VERSION="3.8.3-Leia"
PKG_SHA256="feac27e3bd97d115ad9f3920be6f43447972194246d0f33fa295d9a4f33e4c48"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/kodi-pvr/pvr.iptvsimple"
PKG_URL="https://github.com/kodi-pvr/pvr.iptvsimple/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain kodi-platform rapidxml zlib"
PKG_SECTION=""
PKG_SHORTDESC="pvr.iptvsimple"
PKG_LONGDESC="pvr.iptvsimple"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.pvrclient"

PKG_CMAKE_OPTS_TARGET="-DRAPIDXML_INCLUDE_DIRS=$(get_build_dir rapidxml)"
