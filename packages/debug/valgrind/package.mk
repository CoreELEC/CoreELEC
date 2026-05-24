# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="valgrind"
PKG_VERSION="3.27.1"
PKG_SHA256="5d589152eb8071c02feab8ce6ab719e431a1fbc3e2b1700f5432632a8b9264dc"
PKG_LICENSE="GPL"
PKG_SITE="https://valgrind.org/"
PKG_URL="https://sourceware.org/pub/valgrind/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A tool to help find memory-management problems in programs"
PKG_BUILD_FLAGS="-cfg-libs"

if [ "${TARGET_ARCH}" = "arm" ]; then
  PKG_CONFIGURE_OPTS_TARGET="--enable-only32bit"
elif [ "${TARGET_ARCH}" = "aarch64" -o "${TARGET_ARCH}" = "x86_64" ]; then
  PKG_CONFIGURE_OPTS_TARGET="--enable-only64bit"
fi
