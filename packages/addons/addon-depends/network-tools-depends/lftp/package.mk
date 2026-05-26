# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="lftp"
PKG_VERSION="4.9.3"
PKG_SHA256="96e7199d7935be33cf6b1161e955b2aab40ab77ecdf2a19cea4fc1193f457edc"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="http://lftp.yar.ru/"
PKG_URL="http://lftp.yar.ru/ftp/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain readline openssl zlib libidn2"
PKG_LONGDESC="A sophisticated ftp/http client, and a file transfer program supporting a number of network protocols."
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET="--disable-nls \
                           --disable-rpath \
                           --without-gnutls \
                           --with-openssl \
                           --with-readline=${SYSROOT_PREFIX}/usr \
                           --with-zlib=${SYSROOT_PREFIX}/usr"

post_configure_target() {
  libtool_remove_rpath libtool
}
