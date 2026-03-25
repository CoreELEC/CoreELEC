# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="icu"
PKG_VERSION="78.3"
PKG_SHA256="3a2e7a47604ba702f345878308e6fefeca612ee895cf4a5f222e7955fabfe0c0"
PKG_LICENSE="Custom"
PKG_SITE="https://icu.unicode.org"
PKG_URL="https://github.com/unicode-org/icu/releases/download/release-${PKG_VERSION}/icu4c-${PKG_VERSION}-sources.tgz"
PKG_DEPENDS_HOST="toolchain:host"
PKG_DEPENDS_TARGET="toolchain icu:host"
PKG_LONGDESC="International Components for Unicode library."
PKG_TOOLCHAIN="configure"

PKG_BUILD_FLAGS="-sysroot"

configure_package() {
  PKG_CONFIGURE_SCRIPT="${PKG_BUILD}/source/configure"
  PKG_CONFIGURE_OPTS_TARGET="--disable-layout \
                             --disable-layoutex \
                             --enable-renaming \
                             --disable-samples \
                             --disable-tests \
                             --disable-tools \
                             --with-cross-build=${PKG_BUILD}/.${HOST_NAME}"
}
