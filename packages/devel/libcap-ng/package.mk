# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libcap-ng"
PKG_VERSION="0.9.1"
PKG_SHA256="52418b8940f83dcc00dcd01d187e67c3399ff65f3fa558442e3a21b415cc46c0"
PKG_LICENSE="LGPLv2.1"
PKG_SITE="https://github.com/stevegrubb/libcap-ng"
PKG_URL="https://github.com/stevegrubb/libcap-ng/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Libcap-ng is a library for Linux that makes using posix capabilities easy."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-cfg-libs"

PKG_CONFIGURE_OPTS_TARGET="--enable-static --disable-shared --with-python3=no"
