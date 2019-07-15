# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="fluidsynth-git"
PKG_VERSION="2.0.4"
PKG_SHA256="2c065de87e9c9ba0311ebf2f4828a4fd76f1f5cc7d1d93dd80d7a048d7d2a76c"
PKG_LICENSE="LGPL"
PKG_SITE="http://fluidsynth.org/"
PKG_URL="https://github.com/FluidSynth/fluidsynth/archive/v$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib soundfont-generaluser"
PKG_LONGDESC="FluidSynth is a software real-time synthesizer based on the Soundfont 2 specifications."
PKG_BUILD_FLAGS="+pic"

pre_configure_target() {
  PKG_CMAKE_OPTS_TARGET="-Denable-readline=0 \
                         -Denable-oss=0 \
                         -Denable-pulseaudio=1 \
                         -Denable-libsndfile=0"
}

post_makeinstall_target() {
  # Create directories
  mkdir -p $INSTALL/etc/fluidsynth
  mkdir -p $INSTALL/usr/config/fluidsynth/soundfonts

  # Create symlinks & install config file
  cp $PKG_DIR/config/* $INSTALL/usr/config/fluidsynth/
  ln -s /storage/.config/fluidsynth/fluidsynth.conf $INSTALL/etc/fluidsynth/
  echo "Place your SoundFonts here!" >> $INSTALL/usr/config/fluidsynth/soundfonts/readme.txt

  # Create symlink to SoundFont
  ln -s /usr/share/soundfonts/GeneralUser.sf2  $INSTALL/usr/config/fluidsynth/soundfonts/
}
