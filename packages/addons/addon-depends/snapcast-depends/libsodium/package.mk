# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libsodium"
PKG_VERSION="1.0.21"
PKG_SHA256="bc82498fdeba7c80f97317dc9590a1e02b4a56e27a0d738e921e556f489a1989"
PKG_LICENSE="ISC"
PKG_SITE="https://libsodium.org/"
PKG_URL="https://download.libsodium.org/libsodium/releases/libsodium-${PKG_VERSION}-stable.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A modern, portable, easy to use crypto library"

PKG_CONFIGURE_OPTS_TARGET="--disable-shared"
