# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="double-conversion"
PKG_VERSION="3.4.0"
PKG_SHA256="42fd4d980ea86426e457b24bdfa835a6f5ad9517ddb01cdb42b99ab9c8dd5dc9"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://github.com/google/double-conversion"
PKG_URL="https://github.com/google/double-conversion/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Efficient binary-decimal and decimal-binary conversion routines for IEEE doubles."
PKG_TOOLCHAIN="cmake"
PKG_BUILD_FLAGS="-sysroot"

PKG_CMAKE_OPTS_TARGET="-DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
                       -DBUILD_SHARED_LIBS=ON \
                       -DBUILD_TESTING=OFF"
