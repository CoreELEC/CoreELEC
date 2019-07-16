# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mpv"
PKG_VERSION="dd83b66652d93e5422757f569b084867a9052e48"
PKG_SHA256="8560e948e257a1e301a3a4e4bf2628436db28072791107f1edbced043b29717f"
PKG_LICENSE="GPLv2+"
PKG_SITE="https://github.com/mpv-player/mpv"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git libass"
PKG_LONGDESC="Video player based on MPlayer/mplayer2 https://mpv.io"
PKG_TOOLCHAIN="manual"

configure_target() {
  #./bootstrap.py 
  # the bootstrap was failing for some reason. 
  cp $PKG_DIR/waf/* $PKG_BUILD  
  ./waf configure --enable-sdl2 --disable-pulse --disable-libbluray --disable-drm
}

make_target() {
  ./waf build
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  cp ./build/mpv $INSTALL/usr/bin
}
