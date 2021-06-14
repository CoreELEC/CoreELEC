# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="sdl12-compat"
PKG_VERSION="9ab4eff9c08c9310c5a96ddcfc94100cf8ef65f0"
PKG_SHA256="c9a8047b4bf1b33fd35ebdabc2a580dc63363b56786e08041cb3f04a4264af35"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/libsdl-org/sdl12-compat"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain yasm:host alsa-lib systemd dbus"
PKG_SECTION="multimedia"
PKG_SHORTDESC="SDL: A cross-platform Graphic API"
PKG_LONGDESC="An SDL-1.2 compatibility layer that uses SDL 2.0 behind the scenes. "

post_unpack() {
sed -i "s|SDL_PIXELFORMAT_XRGB8888|SDL_PIXELFORMAT_ARGB8888|" $PKG_BUILD/src/SDL12_compat.c
}

pre_configure_target() {
PKG_CMAKE_OPTS_TARGET+=" -DSDL12TESTS=off"
}

pre_configure_host() {
PKG_CMAKE_OPTS_HOST+=" -DSDL12TESTS=off"
}
