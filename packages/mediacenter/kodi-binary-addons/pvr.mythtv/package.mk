# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.mythtv"
PKG_VERSION="22.3.9-Piers"
PKG_SHA256="c23012febe0e66a7627dfc32c75aa0bf318773029ce9770b5e18ecc69773c4d5"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/janbar/pvr.mythtv"
PKG_URL="https://github.com/janbar/pvr.mythtv/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host zlib"
PKG_SECTION=""
PKG_SHORTDESC="pvr.mythtv"
PKG_LONGDESC="pvr.mythtv"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.pvrclient"
