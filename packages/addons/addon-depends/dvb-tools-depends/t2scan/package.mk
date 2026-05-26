# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="t2scan"
PKG_VERSION="ae1c768d8ff08400f8409e9e9338d375b78731c1"
PKG_SHA256="7a04aaabff34c83bac683e50e27494467ff1865829d2f95445a17228fe4b77c6"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/mighty-p/t2scan"
PKG_URL="https://github.com/mighty-p/t2scan/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A small channel scan tool which generates DVB-T/T2 channels.conf files."
PKG_BUILD_FLAGS="-sysroot -cfg-libs"
