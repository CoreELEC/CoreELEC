# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xxHash"
PKG_VERSION="0.8.2"
PKG_SHA256="baee0c6afd4f03165de7a4e67988d16f0f2b257b51d0e3cb91909302a26a79c4"
PKG_LICENSE="BSD-2-Clause"
PKG_SITE="https://xxhash.com"
PKG_URL="https://github.com/Cyan4973/xxHash/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="make:host"
PKG_LONGDESC="Extremely fast non-cryptographic hash algorithm"
PKG_BUILD_FLAGS="+local-cc"

pre_configure_host() {
  export prefix=${TOOLCHAIN}
}
