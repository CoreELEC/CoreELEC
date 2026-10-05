# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libimobiledevice-glue"
PKG_VERSION="1.3.3"
PKG_SHA256="920ce01382a32695f49b23292b4979a03f0afd16c58e8755d8b7f41804acc1a9"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="http://www.libimobiledevice.org"
PKG_URL="https://github.com/libimobiledevice/libimobiledevice-glue/releases/download/${PKG_VERSION}/libimobiledevice-glue-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain libplist"
PKG_LONGDESC="A library with common code used by libraries and tools around the libimobiledevice project"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-largefile"

post_configure_target() {
  libtool_remove_rpath libtool
}

post_makeinstall_target() {
  mkdir -p "${SYSROOT_PREFIX}/usr/include/lib/libimobiledevice-glue"
    cp ${PKG_BUILD}/include/libimobiledevice-glue/utils.h "${SYSROOT_PREFIX}/usr/include/libimobiledevice-glue"
}
