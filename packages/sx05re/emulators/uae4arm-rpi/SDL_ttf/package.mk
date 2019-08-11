# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="SDL_ttf"
PKG_VERSION="2.0.11"
PKG_SHA256=""
PKG_LICENSE="GPL"
PKG_SITE="http://www.libsdl.org/"
PKG_URL="https://www.libsdl.org/projects/SDL_ttf/release/SDL_ttf-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL"
PKG_LONGDESC="This is a sample library which allows you to use TrueType fonts in your SDL applications"

PKG_CONFIGURE_OPTS_TARGET="--with-freetype-prefix=$SYSROOT_PREFIX/usr"
