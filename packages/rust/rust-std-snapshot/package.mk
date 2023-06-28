# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="rust-std-snapshot"
PKG_VERSION="$(get_pkg_version rust)"
PKG_LICENSE="MIT"
PKG_SITE="https://www.rust-lang.org"
PKG_LONGDESC="rust std library bootstrap package"
PKG_TOOLCHAIN="manual"

case "${MACHINE_HARDWARE_NAME}" in
  "aarch64")
    PKG_SHA256="966e85187b6b76dc520b23aadc886c5fe54b209a21c68f959ff00ef8542b7f9f"
    PKG_URL="https://static.rust-lang.org/dist/rust-std-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
  "arm")
    PKG_SHA256="6ad1231aeb34fef9c9db267859b0db3c6846bbade8227e6c9f456b6264c1278c"
    PKG_URL="https://static.rust-lang.org/dist/rust-std-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnueabihf.tar.xz"
    ;;
  "x86_64")
    PKG_SHA256="0c0129717da1e27ccf2c56da950d2fe56973f71beec9e80ae6904b282d2f0ee9"
    PKG_URL="https://static.rust-lang.org/dist/rust-std-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
esac
PKG_SOURCE_NAME="rust-std-snapshot_${PKG_VERSION}_${ARCH}.tar.xz"
