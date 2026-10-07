# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libbpf"
PKG_VERSION="1.8.0"
PKG_SHA256="b7a1e685f90f6a63ead0dd85d053694b222975da8d09c1a966041cff6f0055ff"
PKG_LICENSE="LGPL-2.1-only OR BSD-2-Clause"
PKG_SITE="https://github.com/libbpf/libbpf"
PKG_URL="https://github.com/libbpf/libbpf/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain elfutils"
PKG_LONGDESC="libbpf supports building BPF CO-RE-enabled applications"
PKG_TOOLCHAIN="make"

make_target() {
  make BUILD_STATIC_ONLY=1 \
       PREFIX=${SYSROOT_PREFIX}/usr \
       -C src
}

makeinstall_target() {
  make BUILD_STATIC_ONLY=1 \
       PREFIX=${SYSROOT_PREFIX}/usr \
       -C src install
}
