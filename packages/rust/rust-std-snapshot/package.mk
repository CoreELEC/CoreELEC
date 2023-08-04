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
    PKG_SHA256="232afbb1b8673694742eba6444ee7c70294ca6da285cdeef9d43acd3f4c58ddc"
    PKG_URL="https://static.rust-lang.org/dist/rust-std-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
  "arm")
    PKG_SHA256="4720dee4efe1cbcf7c21ba3a5806dbfb6f7f04e67dec1a79c3fd61d5fd2deea3"
    PKG_URL="https://static.rust-lang.org/dist/rust-std-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnueabihf.tar.xz"
    ;;
  "x86_64")
    PKG_SHA256="31f392df564850d78be80adc625b06a3964a49ef5c519075b930f2042a422264"
    PKG_URL="https://static.rust-lang.org/dist/rust-std-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
esac
PKG_SOURCE_NAME="rust-std-snapshot_${PKG_VERSION}_${ARCH}.tar.xz"
