# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libimobiledevice"
PKG_VERSION="1.4.0"
PKG_SHA256="23cc0077e221c7d991bd0eb02150a0d49199bcca1ddf059edccee9ffd914939d"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="http://www.libimobiledevice.org"
PKG_URL="https://github.com/libimobiledevice/libimobiledevice/releases/download/${PKG_VERSION}/libimobiledevice-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain libimobiledevice-glue libplist libtatsu libusbmuxd openssl"
PKG_LONGDESC="A cross-platform software library that talks the protocols to support Apple devices."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --without-cython \
                           --disable-largefile"

configure_package() {
  # if using a git hash as a package version - set RELEASE_VERSION
  export RELEASE_VERSION="$(sed -n '1,/RE/s/Version \(.*\)/\1/p' ${PKG_BUILD}/NEWS)-git-${PKG_VERSION:0:7}"
}

post_configure_target() {
  libtool_remove_rpath libtool
}
