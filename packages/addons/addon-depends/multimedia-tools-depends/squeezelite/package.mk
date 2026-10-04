# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="squeezelite"
PKG_VERSION="ab20df3dff76122b9e4f426cffd0346e752b93da"
PKG_SHA256="d5ec9de950ecfd5ab7aa52fd9cac1f07c46e4c62fdfd208ab91ec42145c378e7"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://github.com/ralph-irving/squeezelite"
PKG_URL="https://github.com/ralph-irving/squeezelite/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain faad2 ffmpeg flac libmad libvorbis mpg123 soxr libogg"
PKG_DEPENDS_CONFIG="mpg123"
PKG_LONGDESC="A client for the Lyrion Music Server."
PKG_BUILD_FLAGS="-sysroot"

make_target() {
  make \
    OPTS="-DDSD -DFFMPEG -DRESAMPLE -DVISEXPORT -DLINKALL" \
    CFLAGS="${CFLAGS} $(pkg-config --cflags libmpg123 vorbisfile vorbis ogg)" \
    LDFLAGS+=" $(pkg-config --libs libmpg123 vorbisfile vorbis ogg)"
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  cp -p squeezelite ${INSTALL}/usr/bin
}
