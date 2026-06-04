# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libusbmuxd"
PKG_VERSION="2.1.1"
PKG_SHA256="5546f1aba1c3d1812c2b47d976312d00547d1044b84b6a461323c621f396efce"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="http://www.libimobiledevice.org"
PKG_URL="https://github.com/libimobiledevice/libusbmuxd/releases/download/${PKG_VERSION}/libusbmuxd-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain libimobiledevice-glue libplist"
PKG_LONGDESC="A USB multiplex daemon."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="ac_cv_func_malloc_0_nonnull=yes \
                           ac_cv_func_realloc_0_nonnull=yes \
                           --enable-static \
                           --disable-shared"

configure_package() {
  # if using a git hash as a package version - set RELEASE_VERSION
  if [ -f ${PKG_BUILD}/NEWS ]; then
    export RELEASE_VERSION="$(sed -n '1,/RE/s/Version \(.*\)/\1/p' ${PKG_BUILD}/NEWS)-git-${PKG_VERSION:0:7}"
  fi
}

post_configure_target() {
  libtool_remove_rpath libtool
}
