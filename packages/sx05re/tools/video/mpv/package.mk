# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mpv"
PKG_VERSION="3451908a34437b69cd984198642b4f954cf6c4bf"
PKG_SHA256="aadf762d7ff1417e5ea3710fde2a0b20cba4ab901ed6de8f80e08bf18558e804"
PKG_LICENSE="GPLv2+"
PKG_SITE="https://github.com/mpv-player/mpv"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git"
PKG_LONGDESC="Video player based on MPlayer/mplayer2 https://mpv.io"
PKG_TOOLCHAIN="manual"

configure_target() {
  #./bootstrap.py 
  # the bootstrap was failing for some reason. 
  cp $PKG_DIR/waf/* $PKG_BUILD  
  ./waf configure --enable-sdl2 --enable-sdl2-gamepad --disable-pulse --disable-libbluray --disable-drm --disable-gl
}

make_target() {
  ./waf build
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  cp ./build/mpv $INSTALL/usr/bin
}
