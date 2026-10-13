# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="squeezelite"
PKG_VERSION="4e0ec3188320da7de059c571499e626ff8b3712f"
PKG_SHA256="4968a784fe4c2a36d74d900a34213df12a21b40dc14eb5a8a96a8e2b8d62c8e5"
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
