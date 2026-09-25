# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="shairport-sync"
PKG_VERSION="5.5.2"
PKG_SHA256="abcdb59674b6eedb4f3f6228f3c702e65d2cdc037231b0c48617fd90891b49e9"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/mikebrady/shairport-sync"
PKG_URL="https://github.com/mikebrady/shairport-sync/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain alsa-lib avahi ffmpeg libconfig libdaemon libgcrypt libplist:host libplist libsndfile libsodium openssl popt pulseaudio soxr util-linux xxd:host"
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
