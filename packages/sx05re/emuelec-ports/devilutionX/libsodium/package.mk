# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020 Trond Haugland (github.com/escalade)

PKG_NAME="libsodium"
PKG_VERSION="1.0.18"
PKG_LICENSE="GPL"
PKG_SITE="https://libsodium.org"
PKG_URL="https://download.libsodium.org/libsodium/releases/libsodium-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A modern, portable, easy to use crypto library"
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+speed"

