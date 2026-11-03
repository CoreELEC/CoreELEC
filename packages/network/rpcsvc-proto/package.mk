# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="rpcsvc-proto"
PKG_VERSION="1.4.4"
PKG_SHA256="81c3aa27edb5d8a18ef027081ebb984234d5b5860c65bd99d4ac8f03145a558b"
PKG_LICENSE="BSD"
PKG_SITE="http://nfs.sourceforge.net/"
PKG_URL="https://github.com/thkukuk/rpcsvc-proto/releases/download/v$PKG_VERSION/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_HOST="toolchain"
PKG_DEPENDS_TARGET="toolchain rpcsvc-proto:host"
PKG_LONGDESC="The rpcsvc-proto package contains the rcpsvc protocol.x files and headers."

pre_configure_host() {
  cd $PKG_BUILD
  rm -rf .$HOST_NAME
}

pre_configure_target() {
  cd $PKG_BUILD
  rm -rf .$TARGET_NAME
}

make_host() {
  make rpcgen -C rpcgen
}

makeinstall_target() {
  : #
}
