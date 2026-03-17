# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="giflib"
PKG_VERSION="6.1.2"
PKG_SHA256="2421abb54f5906b14965d28a278fb49e1ec9fe5ebbc56244dd012383a973d5c0"
PKG_LICENSE="OSS"
PKG_SITE="http://giflib.sourceforge.net/"
PKG_URL="${SOURCEFORGE_SRC}/giflib/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="zlib:host"
PKG_DEPENDS_TARGET="toolchain zlib"
PKG_LONGDESC="giflib: giflib service library"
PKG_TOOLCHAIN="manual"

make_host() {
  make libgif.a CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"
}

makeinstall_host() {
  make install-include PREFIX="${TOOLCHAIN}"
  make install-lib PREFIX="${TOOLCHAIN}"
}
