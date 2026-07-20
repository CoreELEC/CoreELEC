# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="kmsxx"
PKG_VERSION="bfb041620d8ee5fbbc2f2432edc658389e5635da"
PKG_SHA256="87032777b7b6c0ed6cd189da7f9b50f8fee3234003c2e54677c83bf8c298b707"
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
