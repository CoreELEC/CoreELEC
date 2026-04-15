# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="giflib"
PKG_VERSION="6.1.3"
PKG_SHA256="b65b66b99f0424b93525f987386f22fc5efb9da2bfc92ad4a532249aaffbab0e"
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
