# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="fluidsynth"
PKG_VERSION="2.5.5"
PKG_SHA256="0827eefc06f66157c332d7bd0d65ee81be5d4c795f214db7ba0e1c70ee394430"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="http://fluidsynth.org/"
PKG_URL="https://github.com/FluidSynth/fluidsynth/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib libsndfile gcem"
PKG_LONGDESC="FluidSynth renders midi music files as raw audio data, for playing or conversion."
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=0 \
                       -Denable-libsndfile=1 \
                       -Denable-pulseaudio=0 \
                       -Denable-readline=0"
