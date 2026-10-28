# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="squeezelite"
PKG_VERSION="afa8e834b2e2774016ca244a5f19f67ed0bd7d3b"
PKG_SHA256="8e5e33a2462b115d87b6bacab73a59c3a940b522cb1b136829c8194b02a782fb"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://github.com/ralph-irving/squeezelite"
PKG_URL="https://github.com/ralph-irving/squeezelite/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain faad2 ffmpeg flac libmad libvorbis mpg123 soxr libogg"
PKG_DEPENDS_CONFIG="libmad mpg123"
PKG_LONGDESC="A client for the Lyrion Music Server."
PKG_BUILD_FLAGS="-sysroot"

make_target() {
  make \
    OPTS="-DDSD -DFFMPEG -DRESAMPLE -DVISEXPORT -DLINKALL" \
    CFLAGS="${CFLAGS} $(pkg-config --cflags libmpg123 mad vorbisfile vorbis ogg)" \
    LDFLAGS+=" $(pkg-config --libs libmpg123 mad vorbisfile vorbis ogg)"
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  cp -p squeezelite ${INSTALL}/usr/bin
}
