# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mpd-mpc"
PKG_VERSION="0.35"
PKG_SHA256="382959c3bfa2765b5346232438650491b822a16607ff5699178aa1386e3878d4"
PKG_LICENSE="GPL"
PKG_SITE="https://www.musicpd.org"
PKG_URL="https://www.musicpd.org/download/mpc/0/mpc-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libiconv libmpdclient"
PKG_LONGDESC="Command-line client for MPD."
PKG_BUILD_FLAGS="-sysroot"
