# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libscfg"
PKG_VERSION="0.2.0"
PKG_SHA256="18bf24666aa77b12347b6dffeec32b2e7d24719698041c372ab27da80d16150c"
PKG_LICENSE="MIT"
PKG_SITE="https://codeberg.org/emersion/libscfg"
PKG_URL="https://codeberg.org/emersion/libscfg/releases/download/v${PKG_VERSION}/libscfg-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A C library for scfg"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="--default-library static"
