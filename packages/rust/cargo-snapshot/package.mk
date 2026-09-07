# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cargo-snapshot"
PKG_VERSION="$(get_pkg_version rust)"
PKG_LICENSE="MIT OR Apache-2.0"
PKG_SITE="https://www.rust-lang.org"
PKG_LONGDESC="cargo bootstrap package"
PKG_TOOLCHAIN="manual"

case "${MACHINE_HARDWARE_NAME}" in
  "aarch64")
    PKG_SHA256="c09425a7f300af148c0bdfded0e7d5f1fe7c075b2b9674e8935afa769297b2f6"
    PKG_URL="https://static.rust-lang.org/dist/cargo-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
  "arm")
    PKG_SHA256="7ab7732a9d043de3a5ea5be328b60b6b28550ff1ca87372d2b3a8801ec9647b5"
    PKG_URL="https://static.rust-lang.org/dist/cargo-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnueabihf.tar.xz"
    ;;
  "x86_64")
    PKG_SHA256="ea1de9f9e23107d97ee2b41a72c552f34064a593da503789218387aee59f3ba4"
    PKG_URL="https://static.rust-lang.org/dist/cargo-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
esac
PKG_SOURCE_NAME="cargo-snapshot_${PKG_VERSION}_${ARCH}.tar.xz"
