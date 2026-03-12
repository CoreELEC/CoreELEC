# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libsodium"
PKG_VERSION="1.0.21"
PKG_SHA256="e80b4a8cd45ac1a7bc8037e2e38300032d9e42d24b963148b6590b4a53bac773"
PKG_LICENSE="ISC"
PKG_SITE="https://libsodium.org/"
PKG_URL="https://download.libsodium.org/libsodium/releases/libsodium-${PKG_VERSION}-stable.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A modern, portable, easy to use crypto library"

PKG_CONFIGURE_OPTS_TARGET="--disable-shared"
