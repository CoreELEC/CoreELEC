# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert/)

PKG_NAME="mupen64plus-video-glide64mk2"
PKG_VERSION="a91b7c9bedc6d7876b7835671a614f9d75eb0086"
PKG_SHA256=""
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/mupen64plus/mupen64plus-video-glide64mk2"
PKG_URL="https://github.com/mupen64plus/mupen64plus-video-glide64mk2/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux glibc freetype:host zlib bzip2 libpng ${OPENGLES} SDL2-git boost"
PKG_LONGDESC="Video plugin for Mupen64Plus 2.0 based on 10th anniversary release code from gonetz"
PKG_TOOLCHAIN="make"

PKG_MAKE_OPTS_TARGET="-f projects/unix/Makefile \
						GLIDEDIR=$(get_build_dir mupen64plus-video-glide64mk2)/src/Glitch64/inc \
						USE_GLES=1 \
						HOST_CPU=arm \
						PIC=1 \
						SRCDIR=src \
						APIDIR=$(get_build_dir mupen64plus-core)/src/api \
						NO_SSE=1 \
						all"
						
pre_configure_target() {
  export SYSROOT_PREFIX=$SYSROOT_PREFIX
  export PREFIX=$SYSROOT_PREFIX
}

makeinstall_target() {
 :
}
