# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libopenmpt"
PKG_VERSION="0.7.4"
PKG_SHA256="1600f9335eae3904089a6286f525812961c54ce36a05dfe6eeaa576dd9328f3f"
PKG_LICENSE="BSD"
PKG_SITE="https://lib.openmpt.org/libopenmpt/"
PKG_URL="https://lib.openmpt.org/files/libopenmpt/src/${PKG_NAME}-${PKG_VERSION}+release.autotools.tar.gz"
PKG_DEPENDS_TARGET="toolchain libogg libvorbis zlib"
PKG_LONGDESC="libopenmpt renders mod music files as raw audio data, for playing or conversion."
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --without-mpg123 \
                           --with-vorbis \
                           --with-vorbisfile \
                           --without-pulseaudio \
                           --without-portaudio \
                           --without-portaudiocpp \
                           --without-sdl \
                           --without-sdl2 \
                           --without-sndfile \
                           --without-flac"
