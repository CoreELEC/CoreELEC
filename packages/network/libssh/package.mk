# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libssh"
PKG_VERSION="0.8.4"
PKG_SHA256="6bb07713021a8586ba2120b2c36c468dc9ac8096d043f9b1726639aa4275b81b"
PKG_LICENSE="LGPL"
PKG_SITE="http://www.libssh.org/"
PKG_URL="https://www.libssh.org/files/${PKG_VERSION%.*}/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain zlib openssl"
PKG_LONGDESC="Library for accessing ssh client services through C libraries."

PKG_CMAKE_OPTS_TARGET="-DWITH_STATIC_LIB=1 \
                       -DWITH_SERVER=OFF \
                       -DWITH_GCRYPT=OFF \
                       -DWITH_GSSAPI=OFF"

makeinstall_target() {
# install static library only
  mkdir -p $SYSROOT_PREFIX/usr/lib
    cp src/libssh.a $SYSROOT_PREFIX/usr/lib

  mkdir -p $SYSROOT_PREFIX/usr/lib/pkgconfig
    cp libssh.pc $SYSROOT_PREFIX/usr/lib/pkgconfig

  mkdir -p $SYSROOT_PREFIX/usr/include/libssh
    cp ../include/libssh/callbacks.h $SYSROOT_PREFIX/usr/include/libssh
    cp ../include/libssh/legacy.h $SYSROOT_PREFIX/usr/include/libssh
    cp ../include/libssh/libssh.h $SYSROOT_PREFIX/usr/include/libssh
    cp ../include/libssh/server.h $SYSROOT_PREFIX/usr/include/libssh
    cp ../include/libssh/sftp.h $SYSROOT_PREFIX/usr/include/libssh
    cp ../include/libssh/ssh2.h $SYSROOT_PREFIX/usr/include/libssh
}
