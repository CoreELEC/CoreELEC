# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="hatarisa"
PKG_VERSION="4b16aa80da52297d897fcc5871f03aa4cc1a91d6"
PKG_SHA256="8d6644e8798bbc49037fa9732ad66d5fed03b0a2d358c37ce9c1ef29454c6b65"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/hatari/hatari"
PKG_URL="https://github.com/hatari/hatari/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux glibc systemd alsa-lib SDL2-git portaudio zlib capsimg libpng"
PKG_LONGDESC="Hatari is an Atari ST/STE/TT/Falcon emulator"


pre_configure_target() {
  PKG_CMAKE_OPTS_TARGET="-DCMAKE_SKIP_RPATH=ON \
                         -DDATADIR="/usr/config/hatari" \
                         -DBIN2DATADIR="../../storage/.config/hatari" \
                         -DCAPSIMAGE_INCLUDE_DIR=$PKG_BUILD/src/include \
                         -DCAPSIMAGE_LIBRARY=$PKG_BUILD/libcapsimage.so.5.1"

  # copy IPF Support Library include files
  mkdir -p $PKG_BUILD/src/includes/caps5/
  cp -R $(get_build_dir capsimg)/LibIPF/* $PKG_BUILD/src/includes/caps5/
  cp -R $(get_build_dir capsimg)/Core/CommonTypes.h $PKG_BUILD/src/includes/caps5/
  cp -R $(get_build_dir capsimg)/.install_pkg/usr/lib/libcapsimage.so.5.1 $PKG_BUILD/

  # add library search path for loading libcapsimage library
  LDFLAGS="$LDFLAGS -Wl,-rpath='$PKG_BUILD'"
}

makeinstall_target() {
  # create directories
  mkdir -p $INSTALL/usr/bin
  mkdir -p $INSTALL/usr/config/hatari

  # copy config files  
  touch $INSTALL/usr/config/hatari/hatari.nvram
  cp -R $PKG_DIR/config/* $INSTALL/usr/config/hatari

  # copy binary & start script
  cp src/hatari $INSTALL/usr/bin
  cp -R $PKG_DIR/scripts/* $INSTALL/usr/bin/
}
