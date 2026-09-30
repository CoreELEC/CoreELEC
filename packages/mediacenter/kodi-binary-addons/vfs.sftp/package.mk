# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="vfs.sftp"
PKG_VERSION="22.0.5-Piers"
PKG_SHA256="64b5da3e0cbb7a45476e2dba68ce5d963a2e0f8b05a1ee1813469a0e18970ef4"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/vfs.sftp"
PKG_URL="https://github.com/xbmc/vfs.sftp/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tinyxml ${MEDIACENTER}:host libssh"
PKG_SECTION=""
PKG_SHORTDESC="vfs.sftp"
PKG_LONGDESC="vfs.sftp"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="kodi.vfs"
