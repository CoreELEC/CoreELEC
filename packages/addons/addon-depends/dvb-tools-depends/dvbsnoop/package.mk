# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dvbsnoop"
PKG_VERSION="d561ddc3c5396e0462f2ef08c19d8fcf4df68f5e"
PKG_SHA256="23fa6d5a7ac74f4ca23598ff91f2ff088011fdd88fa37e2ca3716bf23b90da42"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="http://dvbsnoop.sourceforge.net/"
PKG_URL="https://github.com/Duckbox-Developers/dvbsnoop/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="dvbsnoop is a DVB/MPEG stream analyzer program"
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-sysroot -cfg-libs"
