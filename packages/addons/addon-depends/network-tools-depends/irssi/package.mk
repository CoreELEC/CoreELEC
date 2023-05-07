# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="irssi"
PKG_VERSION="1.4.4"
PKG_SHA256="fefe9ec8c7b1475449945c934a2360ab12693454892be47a6d288c63eb107ead"
PKG_LICENSE="GPL"
PKG_SITE="http://www.irssi.org/"
PKG_URL="https://github.com/irssi/irssi/releases/download/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain glib ncurses openssl"
PKG_LONGDESC="Irssi is a terminal based IRC client for UNIX systems."
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dwithout-textui=no \
                       -Dwith-bot=no \
                       -Dwith-proxy=no \
                       -Dwith-perl=no"
