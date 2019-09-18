# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="mpg123-compat"
PKG_VERSION="1.25.10"
PKG_SHA256="6c1337aee2e4bf993299851c70b7db11faec785303cfca3a5c3eb5f329ba7023"
PKG_LICENSE="LGPLv2"
PKG_SITE="http://www.mpg123.org/"
PKG_URL="http://downloads.sourceforge.net/sourceforge/mpg123/mpg123-$PKG_VERSION.tar.bz2"
PKG_DEPENDS_TARGET="toolchain alsa-lib SDL2-git"
PKG_LONGDESC="A console based real time MPEG Audio Player for Layer 1, 2 and 3."
PKG_BUILD_FLAGS="-fpic"

if [ "$PULSEAUDIO_SUPPORT" = yes ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET pulseaudio"
  PKG_CONFIGURE_OPTS_TARGET="--with-default-audio=pulse --with-audio=alsa,pulse"
fi
