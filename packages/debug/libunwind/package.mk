# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libunwind"
PKG_VERSION="1.8.0"
PKG_SHA256="5bddae0096d5e66bf6299e46d31f7c727bdf9fccbdbd8e1b6ffd9b1f6142cd2e"
PKG_LICENSE="GPL"
PKG_SITE="https://www.nongnu.org/libunwind/"
PKG_URL="https://github.com/libunwind/libunwind/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain zlib"
PKG_LONGDESC="library to determine the call-chain of a program"
PKG_BUILD_FLAGS="+pic"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-minidebuginfo \
                           --disable-documentation \
                           --disable-tests"

makeinstall_target() {
  make DESTDIR=${SYSROOT_PREFIX} install
}
