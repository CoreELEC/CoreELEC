# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libsndfile"
PKG_VERSION="1.2.2"
PKG_SHA256="3799ca9924d3125038880367bf1468e53a1b7e3686a934f098b7e1d286cdb80e"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://libsndfile.github.io/libsndfile/"
PKG_URL="https://github.com/libsndfile/libsndfile/releases/download/${PKG_VERSION}/libsndfile-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain alsa-lib flac libogg libvorbis opus"
PKG_LONGDESC="A C library for reading and writing sound files containing sampled audio data."
PKG_BUILD_FLAGS="+pic"

# As per notes in configure.ac:
#  One or more of the external libraries (ie libflac, libogg, libvorbis and libopus)
#  is either missing ... Unfortunately, for ease of maintenance, the external libs
#  are an all or nothing affair.
# So all of flac, libogg, libvorbis, opus are required.

PKG_CMAKE_OPTS_TARGET="-DBUILD_PROGRAMS=OFF \
                       -DBUILD_EXAMPLES=OFF \
                       -DBUILD_REGTEST=OFF \
                       -DBUILD_TESTING=OFF \
                       -DENABLE_EXTERNAL_LIBS=ON \
                       -DINSTALL_MANPAGES=OFF \
                       -DINSTALL_PKGCONFIG_MODULE=ON"
