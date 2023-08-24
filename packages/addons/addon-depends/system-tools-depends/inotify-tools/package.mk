# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="inotify-tools"
PKG_VERSION="4.23.8.0"
PKG_SHA256="8ad8b72a146af57688f3289b33b92a026915fc677997147071887b65b603d20a"
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
