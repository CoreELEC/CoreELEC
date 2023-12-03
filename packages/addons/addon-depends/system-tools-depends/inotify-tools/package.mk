# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="inotify-tools"
PKG_VERSION="4.23.9.0"
PKG_SHA256="1dfa33f80b6797ce2f6c01f454fd486d30be4dca1b0c5c2ea9ba3c30a5c39855"
PKG_LICENSE="GPLv2"
PKG_SITE="http://wiki.github.com/inotify-tools/inotify-tools/"
PKG_URL="https://github.com/inotify-tools/inotify-tools/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A C library and a set of command-line programs for Linux providing a simple interface to inotify."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET="--enable-static --disable-shared --disable-doxygen"

pre_configure_target() {
  CXXFLAGS+=" -Wno-error=unused-parameter"
}

post_configure_target() {
  libtool_remove_rpath libtool
}
