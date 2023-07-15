# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="rustc-snapshot"
PKG_VERSION="$(get_pkg_version rust)"
PKG_LICENSE="MIT"
PKG_SITE="https://www.rust-lang.org"
PKG_LONGDESC="rustc bootstrap compiler"
PKG_TOOLCHAIN="manual"

case "${MACHINE_HARDWARE_NAME}" in
  "aarch64")
    PKG_SHA256="e61b6e34df8c3a002798a9f627c4da701d66f9fc066a70264e354b03d06e6722"
    PKG_URL="https://static.rust-lang.org/dist/rustc-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
  "arm")
    PKG_SHA256="c24287f1e694bc51080fa7d26e3c08a8971978e72f8c6184a2d61f58f7744df8"
    PKG_URL="https://static.rust-lang.org/dist/rustc-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnueabihf.tar.xz"
    ;;
  "x86_64")
    PKG_SHA256="c293d906769671d1cd18e945671bbd14e0b8a41df5075c47f33e6086fc8a1558"
    PKG_URL="https://static.rust-lang.org/dist/rustc-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
esac
PKG_SOURCE_NAME="rustc-snapshot_${PKG_VERSION}_${ARCH}.tar.xz"
