# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2025-present Team CoreELEC (https://coreelec.org)

PKG_NAME="btop"
PKG_VERSION="1.4.7"
PKG_SHA256="933de2e4d1b2211a638be463eb6e8616891bfba73aef5d38060bd8319baeefc6"
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
