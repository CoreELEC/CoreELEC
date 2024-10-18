# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cargo-snapshot"
PKG_VERSION="$(get_pkg_version rust)"
PKG_LICENSE="MIT"
PKG_SITE="https://www.rust-lang.org"
PKG_LONGDESC="cargo bootstrap package"
PKG_TOOLCHAIN="manual"

case "${MACHINE_HARDWARE_NAME}" in
  "aarch64")
    PKG_SHA256="05c0d904a82cddb8a00b0bbdd276ad7e24dea62a7b6c380413ab1e5a4ed70a56"
    PKG_URL="https://static.rust-lang.org/dist/cargo-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
  "arm")
    PKG_SHA256="eb98ac225f08ebd50eaed439eca687ba074344bd50ae7d95fff9a07a7ccc0899"
    PKG_URL="https://static.rust-lang.org/dist/cargo-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnueabihf.tar.xz"
    ;;
  "x86_64")
    PKG_SHA256="97aeae783874a932c4500f4d36473297945edf6294d63871784217d608718e70"
    PKG_URL="https://static.rust-lang.org/dist/cargo-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
esac
PKG_SOURCE_NAME="cargo-snapshot_${PKG_VERSION}_${ARCH}.tar.xz"
