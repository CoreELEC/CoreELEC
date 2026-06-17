# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="stress-ng"
PKG_VERSION="0.21.03"
PKG_SHA256="6db7089ad9cc2c13d0aa1cf8755112adac92858f4582a46c765bb6b7e1c3e1af"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/ColinIanKing/stress-ng"
PKG_URL="https://github.com/ColinIanKing/stress-ng/archive/refs/tags/V${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain attr keyutils libaio libcap zlib"
PKG_LONGDESC="stress-ng will stress test a computer system in various selectable ways"
PKG_BUILD_FLAGS="-sysroot"
