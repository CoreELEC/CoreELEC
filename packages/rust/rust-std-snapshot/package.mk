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
    PKG_SHA256="41d259c6f84280fd0e7719fea03a7583ba54e33e8ac32a2a7b703ffb0aebb7d9"
    PKG_URL="https://static.rust-lang.org/dist/rust-std-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
  "arm")
    PKG_SHA256="4ef1c8df3dbc0160212afafc8bf643e5bbb40b0147b163f2f5e964a52fc8c217"
    PKG_URL="https://static.rust-lang.org/dist/rust-std-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnueabihf.tar.xz"
    ;;
  "x86_64")
    PKG_SHA256="36f27513a6e4381f15b0cd14097c885af537f990cb6193cec3337c429367bf23"
    PKG_URL="https://static.rust-lang.org/dist/rust-std-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
    ;;
esac
PKG_SOURCE_NAME="rust-std-snapshot_${PKG_VERSION}_${ARCH}.tar.xz"
