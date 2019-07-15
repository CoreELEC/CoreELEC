# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="m64p"
PKG_VERSION="1.0"
PKG_SITE="https://m64p.github.io/"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain mupen64plus-core mupen64plus-rsp-hle mupen64plus-input-sdl mupen64plus-audio-sdl2 mupen64plus-ui-console libpng16 mupen64plus-v20-gliden64 mupen64plus-video-glide64mk2"
PKG_LONGDESC="mupen64plus + GLideN64 + a GUI"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  # Create directories
  mkdir -p $INSTALL/usr/bin
  mkdir -p $INSTALL/usr/config/mupen64plus
  mkdir -p $INSTALL/usr/lib/mupen64plus

  # Install binaries & scripts
  cp $(get_build_dir mupen64plus-ui-console)/mupen64plus $INSTALL/usr/bin/mupen64plus
  cp $PKG_DIR/scripts/*                              $INSTALL/usr/bin

  # Install config files
  cp $PKG_DIR/config/*                             $INSTALL/usr/config/mupen64plus
  cp $(get_build_dir mupen64plus-v20-gliden64)/ini/*.ini           $INSTALL/usr/config/mupen64plus
  cp $(get_build_dir mupen64plus-core)/data/*      $INSTALL/usr/config/mupen64plus
  cp $(get_build_dir mupen64plus-input-sdl)/data/* $INSTALL/usr/config/mupen64plus
  cp $(get_build_dir mupen64plus-video-glide64mk2)/data/* $INSTALL/usr/config/mupen64plus

  # Install libs
  cp $(get_build_dir mupen64plus-v20-gliden64)/libmupen64plus-video-GLideN64.so $INSTALL/usr/lib/mupen64plus
  cp $(get_build_dir mupen64plus-video-glide64mk2)/*-video-glide64mk2.so $INSTALL/usr/lib/mupen64plus
  cp $(get_build_dir mupen64plus-audio-sdl2)/*.so  $INSTALL/usr/lib/mupen64plus
  cp -P $(get_build_dir mupen64plus-core)/*.so*    $INSTALL/usr/lib/mupen64plus
  cp $(get_build_dir mupen64plus-input-sdl)/*.so   $INSTALL/usr/lib/mupen64plus
  cp $(get_build_dir mupen64plus-rsp-hle)/*.so     $INSTALL/usr/lib/mupen64plus
  cp $PKG_DIR/batocera/mupen64plus-video-gliden64.so $INSTALL/usr/lib/mupen64plus
    }
