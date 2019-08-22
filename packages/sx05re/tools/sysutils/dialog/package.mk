# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="dialog"
PKG_VERSION="1.3-20190211"
PKG_SHA256="49c0e289d77dcd7806f67056c54f36a96826a9b73a53fb0ffda1ffa6f25ea0d3"
PKG_LICENSE="GNU-2.1"
PKG_SITE="https://invisible-mirror.net/archives/dialog"
PKG_URL="$PKG_SITE/dialog-$PKG_VERSION.tgz"
PKG_DEPENDS_TARGET="toolchain ncurses"
PKG_LONGDESC="This version of dialog, formerly known as cdialog is based on the Debian package for dialog 0.9a"
PKG_TOOLCHAIN="auto"

PKG_CONFIGURE_OPTS_TARGET="--with-ncurses --disable-rpath-hack"


