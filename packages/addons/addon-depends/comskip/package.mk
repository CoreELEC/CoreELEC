# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="comskip"
PKG_VERSION="0.83"
PKG_SHA256="bd90d7922916e0b04ea9f3426ea7747d347f218f3f915fb4d251961d0730876e"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="http://www.kaashoek.com/comskip/"
PKG_URL="https://github.com/erikkaashoek/Comskip/archive/V${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain argtable2 ffmpegx lame"
PKG_DEPENDS_CONFIG="argtable2 ffmpegx"
PKG_LONGDESC="Comskip detects commercial breaks from a video stream. It can be used for post-processing recordings."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-sysroot -cfg-libs"

pre_configure_target() {
  # lame is a -sysroot package, so ffmpegx's static libavcodec.a (which
  # pulls in libmp3lame symbols) can't be resolved without lame's
  # install path explicitly added here too.
  LDFLAGS+=" -L$(get_install_dir lame)/usr/lib"

  # pass ffmpegx to build
  CFLAGS+=" -I$(get_install_dir ffmpegx)/usr/local/include"
  LDFLAGS+=" -L$(get_install_dir ffmpegx)/usr/local/lib -ldl"
}
