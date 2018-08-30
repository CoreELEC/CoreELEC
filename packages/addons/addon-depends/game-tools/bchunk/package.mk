# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bchunk"
PKG_VERSION="1.2.0"
PKG_SHA256="afdc9d5e38bdd16f0b8b9d9d382b0faee0b1e0494446d686a08b256446f78b5d"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://he.fi/bchunk/"
PKG_URL="http://he.fi/bchunk/$PKG_NAME-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="system"
PKG_SHORTDESC="binchunker converts a CD image in a .bin / .cue format (sometimes .raw / .cue) to a set of .iso and .cdr tracks"
PKG_LONGDESC="binchunker converts a CD image in a .bin / .cue format (sometimes .raw / .cue) to a set of .iso and .cdr tracks"

makeinstall_target() {
  :
}

make_target() {
  make $PKG_MAKE_OPTS_TARGET CC=$CC LD=$CC
}
