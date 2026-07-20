# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="htop"
PKG_VERSION="3.5.2"
PKG_SHA256="a66a62bbd1eba59889c68f868b643e53320eea93da19f43ba13c822a826d82ba"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://hisham.hm/htop"
PKG_URL="https://github.com/htop-dev/htop/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain ncurses"
PKG_LONGDESC="An interactive process viewer for Unix."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-sysroot -cfg-libs"

PKG_CONFIGURE_OPTS_TARGET="--disable-unicode \
                           --disable-static \
                           HTOP_NCURSES_CONFIG_SCRIPT=ncurses-config"

pre_configure_target() {
  export LDFLAGS="${LDFLAGS} -pthread"
}
