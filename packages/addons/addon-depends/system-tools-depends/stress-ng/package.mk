# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="stress-ng"
PKG_VERSION="0.22.01"
PKG_SHA256="67e75894da3f634b85992069b0a888893221a77a3db5e3293cdddd0b1c0d4705"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/ColinIanKing/stress-ng"
PKG_URL="https://github.com/ColinIanKing/stress-ng/archive/refs/tags/V${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain attr keyutils libaio libcap zlib"
PKG_LONGDESC="stress-ng will stress test a computer system in various selectable ways"
PKG_BUILD_FLAGS="-sysroot"
