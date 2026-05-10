# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="kmsxx"
PKG_VERSION="73a82c3afb9af0a392d9d572ed2f6cd1e091a487"
PKG_SHA256="3976d34acecdcc83f263922702e1dbe295ebcdf1c989d0197d811ddd4dc0b6da"
PKG_LICENSE="MPL-2.0"
PKG_SITE="https://github.com/tomba/kmsxx"
PKG_URL="https://github.com/tomba/kmsxx/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libdrm libfmt"
PKG_LONGDESC="Library and utilities for kernel mode setting"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Ddefault_library=shared \
                       -Dkmscube=false \
                       -Domap=disabled \
                       -Dpykms=disabled"
