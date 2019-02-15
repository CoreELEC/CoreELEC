# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libnfs"
PKG_VERSION="030814506e529ef1f1572c7b6e3fc2e4c10cb544"
PKG_SHA256="76e96ac63dc906b4da3f09462322261392f204cdac7bd65dc718615eb6abfab9"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/sahlberg/libnfs"
PKG_URL="https://github.com/sahlberg/libnfs/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A client library for accessing NFS shares over a network."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--disable-examples"

pre_configure_target() {
  export CFLAGS="$CFLAGS -D_FILE_OFFSET_BITS=64"
}
