# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="shairport-sync"
PKG_VERSION="5.0.4"
PKG_SHA256="b89d4af74cffadd83d1be6eaf4e967180aa5a6aed32f561c937ae1d787909c25"
PKG_LICENSE="OSS"
PKG_SITE="https://github.com/mikebrady/shairport-sync"
PKG_URL="https://github.com/mikebrady/shairport-sync/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain alsa-lib avahi ffmpeg libconfig libdaemon libgcrypt libplist:host libplist libsndfile libsodium nqptp openssl popt pulseaudio soxr util-linux xxd:host"
PKG_LONGDESC="AirPlay audio player."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-sysroot -cfg-libs"

PKG_CONFIGURE_OPTS_TARGET="--with-alsa \
                           --with-avahi \
                           --with-convolution \
                           --with-metadata \
                           --with-pa \
                           --with-pipe \
                           --with-pkg-config \
                           --with-soxr \
                           --with-ssl=openssl \
                           --with-stdout \
                           --without-configfiles \
                           --with-airplay-2"
