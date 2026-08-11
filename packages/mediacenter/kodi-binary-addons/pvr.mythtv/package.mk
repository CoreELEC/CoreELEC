# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pvr.mythtv"
PKG_VERSION="22.3.8-Piers"
PKG_SHA256="98a3fc1452851a4fe65754905791a3cfd66b74312a5732a3a9e32699e0f3f272"
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
