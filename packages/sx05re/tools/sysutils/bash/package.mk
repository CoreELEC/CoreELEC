# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bash"
PKG_VERSION="5.1.8"
PKG_SHA256="0cfb5c9bb1a29f800a97bd242d19511c997a1013815b805e0fdd32214113d6be"
PKG_LICENSE="GPL"
PKG_SITE="http://www.gnu.org/software/bash/bash.html"
PKG_URL="https://ftp.gnu.org/gnu/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain ncurses"
PKG_LONGDESC="The GNU Bourne Again shell."

PKG_CONFIGURE_OPTS_TARGET="--with-curses \
                           --enable-readline \
                           --without-bash-malloc \
                           --with-installed-readline"
