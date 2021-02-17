# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="htop"
PKG_VERSION="ce6d60e7def146c13d0b8bca4642e7401a0a8995"
PKG_SHA256="123476d56a5e6a219654eebb6b2ec747dfa364f39c01a6475bf8030a25c81bff"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/htop-dev/htop"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain ncurses"
PKG_LONGDESC="An interactive process viewer for Unix."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--disable-unicode \
                           HTOP_NCURSES_CONFIG_SCRIPT=ncurses-config"
