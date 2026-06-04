# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="stress-ng"
PKG_VERSION="0.21.02"
PKG_SHA256="71502d31f77987c3bda6931d47b44d7b03d8ac689e8154bbda676cf9d42e2b4e"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/ColinIanKing/stress-ng"
PKG_URL="https://github.com/ColinIanKing/stress-ng/archive/refs/tags/V${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain attr keyutils libaio libcap zlib"
PKG_LONGDESC="stress-ng will stress test a computer system in various selectable ways"
PKG_BUILD_FLAGS="-sysroot"
