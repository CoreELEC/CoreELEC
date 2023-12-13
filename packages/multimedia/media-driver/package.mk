# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="media-driver"
PKG_VERSION="23.4.3"
PKG_SHA256="83b95eefe86c9d58d92c2a77793541ea3cb643dff419599ffa87899fd58738cd"
PKG_ARCH="x86_64"
PKG_LICENSE="MIT"
PKG_SITE="https://01.org/linuxmedia"
PKG_URL="https://github.com/intel/media-driver/archive/intel-media-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libva libdrm gmmlib"
PKG_LONGDESC="media-driver: The Intel(R) Media Driver for VAAPI is a new VA-API (Video Acceleration API) user mode driver supporting hardware accelerated decoding, encoding, and video post processing for GEN based graphics hardware."

PKG_CMAKE_OPTS_TARGET="-DBUILD_CMRTLIB=OFF \
                       -DBUILD_KERNELS=ON \
                       -DBUILD_TYPE=release \
                       -DENABLE_NONFREE_KERNELS=ON \
                       -DMEDIA_BUILD_FATAL_WARNINGS=ON \
                       -DMEDIA_RUN_TEST_SUITE=OFF"
