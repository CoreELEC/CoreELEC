# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mpv"
PKG_VERSION="b83bdd1d17cc90b4d8cd2a32321cd7c5cc306422"
PKG_SHA256="9b7162f07c5a516ca3cba7a2029eab7ac66c7929e0a8cba484a3ecd67b8c7177"
PKG_LICENSE="GPLv2+"
PKG_SITE="https://github.com/mpv-player/mpv"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain ffmpeg SDL2-git youtube-dl"
PKG_LONGDESC="Video player based on MPlayer/mplayer2 https://mpv.io"
PKG_TOOLCHAIN="manual"

configure_target() {
  #./bootstrap.py 
  # the bootstrap was failing for some reason. 
  cp $PKG_DIR/waf/* $PKG_BUILD  
  ./waf configure --enable-sdl2 --enable-sdl2-gamepad --disable-pulse --enable-egl --disable-libbluray --disable-drm --disable-gl
}

make_target() {
  ./waf build
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  cp ./build/mpv $INSTALL/usr/bin
}
