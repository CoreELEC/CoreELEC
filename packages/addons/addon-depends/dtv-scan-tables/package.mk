# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dtv-scan-tables"
PKG_VERSION="2026-04-11-be35975ac877"
PKG_SHA256="653ce8dad61e7ba7ba90dca6e88465eeef035ed9deeb2c9f6c9cbc9ed841297c"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://git.linuxtv.org/dtv-scan-tables.git"
PKG_URL="https://linuxtv.org/downloads/dtv-scan-tables/dtv-scan-tables-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Digital TV scan tables."
PKG_TOOLCHAIN="manual"
PKG_BUILD_FLAGS="-sysroot"

makeinstall_target() {
  make -C share/dvb install DATADIR=${INSTALL}/usr/share
}
