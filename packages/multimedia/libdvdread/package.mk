# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libdvdread"
PKG_VERSION="7.0.1"
PKG_SHA256="2e3e04a305c15c3963aa03ae1b9a83c1d239880003fcf3dde986d3943355d407"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://www.videolan.org/"
PKG_URL="https://download.videolan.org/pub/videolan/${PKG_NAME}/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libdvdread is a library which provides a simple foundation for reading DVDs."
PKG_TOOLCHAIN="manual"

if [ "${KODI_DVDCSS_SUPPORT}" = yes ]; then
  PKG_DEPENDS_TARGET+=" libdvdcss"
fi
