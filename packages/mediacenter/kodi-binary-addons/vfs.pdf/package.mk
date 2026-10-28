# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="vfs.pdf"
PKG_VERSION="22.0.2-Piers"
PKG_SHA256="42ebb7ea44e79b3d755294a4f8920e5f581eab880431d924947e66d43c79abd4"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/vfs.pdf"
PKG_URL="https://github.com/xbmc/vfs.pdf/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain ${MEDIACENTER}:host poppler libjpeg-turbo"
PKG_SECTION=""
PKG_SHORTDESC="vfs.pdf"
PKG_LONGDESC="Opens a PDF as a folder of page pictures, so game manuals can be read in Kodi."

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="kodi.vfs"
