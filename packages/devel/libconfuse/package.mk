# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libconfuse"
PKG_VERSION="3.4"
PKG_SHA256="be97bf64ab8052ef874e6c2cfabd2dacf7ad76575bb12c7c42df3fd23675d151"
PKG_LICENSE="ISC"
PKG_SITE="https://github.com/libconfuse/libconfuse"
PKG_URL="https://github.com/libconfuse/libconfuse/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Small configuration file parser library for C"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-static --disable-shared"
