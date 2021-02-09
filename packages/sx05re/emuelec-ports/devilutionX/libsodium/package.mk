# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020 Trond Haugland (github.com/escalade)

PKG_NAME="libsodium"
PKG_VERSION="45bca21a954799d64d9ad86f245916737cdb9971"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/jedisct1/libsodium"
PKG_URL="https://github.com/jedisct1/libsodium/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A modern, portable, easy to use crypto library"
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+speed"

PKG_CONFIGURE_OPTS_TARGET=" --disable-neon"
