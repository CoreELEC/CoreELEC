# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dvblast"
PKG_VERSION="3.5"
PKG_SHA256="6663cf14766c95e5ecf49105baff6ae9c9a5e28d3934a02a368c6785203f7a1e"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://www.videolan.org/projects/dvblast.html"
PKG_URL="https://code.videolan.org/videolan/dvblast/-/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain bitstream libev"
PKG_LONGDESC="DVBlast is a simple and powerful MPEG-2/TS demux and streaming application"
PKG_BUILD_FLAGS="-sysroot"

pre_configure_target() {
  export LDFLAGS="${LDFLAGS} -lm"
  export PREFIX="/usr"
}
