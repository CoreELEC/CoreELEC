# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libsodium"
PKG_VERSION="1.0.22"
PKG_SHA256="eb1ca2b91c035d34ff980e2c5d290bbc57bf0a6ff9b7c8a990f65c89d71abbc0"
PKG_LICENSE="ISC"
PKG_SITE="https://libsodium.org/"
PKG_URL="https://download.libsodium.org/libsodium/releases/libsodium-${PKG_VERSION}-stable.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A modern, portable, easy to use crypto library"

PKG_CONFIGURE_OPTS_TARGET="--disable-shared"
