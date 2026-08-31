# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="imagedecoder.svg"
PKG_VERSION="22.0.1-Piers"
PKG_SHA256="69c20cd52f55da95f0465b2ab6cc8a99b57f2f35796c77dc8bf953e4df2dd06c"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/imagedecoder.svg"
PKG_URL="https://github.com/xbmc/imagedecoder.svg/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain lunasvg ${MEDIACENTER}:host"
PKG_SECTION=""
PKG_SHORTDESC="imagedecoder.svg"
PKG_LONGDESC="imagedecoder.svg"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="kodi.imagedecoder"

PKG_CMAKE_OPTS_TARGET="-Dlunasvg_DIR=$(get_install_dir lunasvg)/usr/lib/cmake/lunasvg \
                       -Dplutovg_DIR=$(get_install_dir lunasvg)/usr/lib/cmake/plutovg"
