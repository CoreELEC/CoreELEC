# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xxHash"
PKG_VERSION="0.8.4"
PKG_SHA256="5738270935e7c3d38a79b3adf7c9692566ce7895a25f67de43ad52ab504acd32"
PKG_LICENSE="BSD-2-Clause"
PKG_SITE="https://xxhash.com"
PKG_URL="https://github.com/Cyan4973/xxHash/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="make:host"
PKG_LONGDESC="Extremely fast non-cryptographic hash algorithm"
PKG_BUILD_FLAGS="+local-cc"

pre_configure_host() {
  export prefix=${TOOLCHAIN}
}
