# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="valgrind"
PKG_VERSION="3.28.0"
PKG_SHA256="9f7a18d128f05d3a80fea53e6953f34aad7200a425ceaee46099a8a798a7dd9c"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://valgrind.org/"
PKG_URL="https://sourceware.org/pub/valgrind/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A tool to help find memory-management problems in programs"
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-cfg-libs"

if [ "${TARGET_ARCH}" = "arm" ]; then
  PKG_CONFIGURE_OPTS_TARGET="--enable-only32bit"
elif [ "${TARGET_ARCH}" = "aarch64" -o "${TARGET_ARCH}" = "x86_64" ]; then
  PKG_CONFIGURE_OPTS_TARGET="--enable-only64bit"
fi
