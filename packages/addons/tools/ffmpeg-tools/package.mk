# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ffmpeg-tools"
PKG_VERSION="1.0"
PKG_REV="104"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://libreelec.tv"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain ffmpegx"
PKG_SECTION="tools"
PKG_SHORTDESC="FFmpeg binary for transcoding and audio/video manipulating."
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="FFmpeg Tools"
PKG_ADDON_TYPE="xbmc.python.script"

addon() {
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID/bin/
  cp -L $(get_build_dir ffmpegx)/.INSTALL_PKG/usr/local/bin/* $ADDON_BUILD/$PKG_ADDON_ID/bin
}
