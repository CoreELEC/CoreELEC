# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libseccomp"
PKG_VERSION="2.6.1"
PKG_SHA256="501f66c667225d53791b97e1d7cf85ab764c297d04881f60f38f451c4b0ee1be"
PKG_LICENSE="LGPL-2.1-only"
PKG_SITE="https://github.com/seccomp/libseccomp"
PKG_URL="https://github.com/seccomp/libseccomp/releases/download/v${PKG_VERSION}/libseccomp-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="An easy to use, platform independent, interface to the Linux Kernel syscall filtering mechanism"
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET+=" --enable-static --enable-shared"
