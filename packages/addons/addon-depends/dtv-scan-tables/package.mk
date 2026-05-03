# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dtv-scan-tables"
PKG_VERSION="2026-03-16-7a595d06a6b0"
PKG_SHA256="1de931966450983fd0e85d1b4198ac475f895839f26e686796c2ad31ffc5fe6f"
PKG_LICENSE="GPL"
PKG_SITE="https://git.linuxtv.org/dtv-scan-tables.git"
PKG_URL="https://linuxtv.org/downloads/dtv-scan-tables/dtv-scan-tables-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Digital TV scan tables."
PKG_TOOLCHAIN="manual"
PKG_BUILD_FLAGS="-sysroot"

makeinstall_target() {
  make -C share/dvb install DATADIR=${INSTALL}/usr/share
}
