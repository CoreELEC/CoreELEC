# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libimobiledevice"
PKG_VERSION="73b6fd183872096f20e6d1007429546a317a7cb1"
PKG_SHA256="854cb9e2671fdf5a2fca609e5e247ab498218b1659a2e5e2b47cf7c1392cec46"
PKG_LICENSE="GPL"
PKG_SITE="http://www.libimobiledevice.org"
PKG_URL="https://github.com/libimobiledevice/libimobiledevice/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libimobiledevice-glue libplist libusbmuxd openssl"
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
