# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dtv-scan-tables"
PKG_VERSION="2024-03-24-7098bdd27548"
PKG_SHA256="562b362f04a83940d1882049488c0c3c4f9dd5d1c2129d1af5b00cfa24493007"
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
