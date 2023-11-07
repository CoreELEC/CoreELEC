# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libuv"
PKG_VERSION="1.47.0"
PKG_SHA256="d50af7e6d72526db137e66fad812421c8a1cae09d146b0ec2bb9a22c5f23ba93"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/libuv/libuv"
PKG_URL="https://github.com/libuv/libuv/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Multi-platform support library with a focus on asynchronous I/O"

PKG_CMAKE_OPTS_TARGET="-DLIBUV_BUILD_TESTS=OFF"
