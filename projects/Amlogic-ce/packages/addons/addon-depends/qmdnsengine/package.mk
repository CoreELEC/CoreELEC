# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="qmdnsengine"
PKG_VERSION="9de38dfbd1cb989b977ed80c512187f0775abbbd"
PKG_SHA256=""
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/nitroshare/qmdnsengine"
PKG_URL="https://github.com/nitroshare/qmdnsengine/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain qt6"
PKG_LONGDESC="Library provides an implementation of multicast DNS as per RFC 6762."

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -DCMAKE_POLICY_VERSION_MINIMUM=3.5"
