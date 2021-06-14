# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="sdl12-compat"
PKG_VERSION="c5bd71de527b981ec736dbd94e973cc11de0cbc0"
PKG_SHA256="7f875f1a5a742dc5656ef8a500d123c938bb27f014ec51591c8e96db2e9641ae"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/libsdl-org/sdl12-compat"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain yasm:host alsa-lib systemd dbus"
PKG_SECTION="multimedia"
PKG_SHORTDESC="SDL: A cross-platform Graphic API"
PKG_LONGDESC="An SDL-1.2 compatibility layer that uses SDL 2.0 behind the scenes. "

pre_configure_target() {
PKG_CMAKE_OPTS_TARGET+=" -DSDL12TESTS=off"
}

pre_configure_host() {
PKG_CMAKE_OPTS_HOST+=" -DSDL12TESTS=off"
}
