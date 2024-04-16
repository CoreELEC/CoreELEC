# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="fluidsynth"
PKG_VERSION="2.3.5"
PKG_SHA256="f89e8e983ecfb4a5b4f5d8c2b9157ed18d15ed2e36246fa782f18abaea550e0d"
PKG_LICENSE="GPL"
PKG_SITE="http://fluidsynth.org/"
PKG_URL="https://github.com/FluidSynth/fluidsynth/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib libsndfile"
PKG_LONGDESC="FluidSynth renders midi music files as raw audio data, for playing or conversion."
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=0 \
                       -Denable-libsndfile=1 \
                       -Denable-pulseaudio=0 \
                       -Denable-readline=0"
