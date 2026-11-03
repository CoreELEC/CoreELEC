# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2025-present Team CoreELEC (https://coreelec.org)

PKG_NAME="btop"
PKG_VERSION="1.4.8"
PKG_SHA256="67e07c21b770a6d884c5e50ba1d2cfc90732ea57459367d8732ec7ecbeb3be5f"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/aristocratos/btop"
PKG_URL="https://github.com/aristocratos/btop/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="btop resource monitor"
PKG_TOOLCHAIN="cmake"

PKG_CMAKE_OPTS_TARGET="-DCMAKE_BUILD_TYPE=Release
                       -DBUILD_TESTING=OFF"

post_makeinstall_target() {
  cat >${INSTALL}/usr/share/btop/btop.conf <<EOF
disks_filter = "/flash /storage"
use_fstab = False
update_ms = 1000
EOF
}
