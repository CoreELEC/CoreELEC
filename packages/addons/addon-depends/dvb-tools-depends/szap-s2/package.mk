# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="szap-s2"
PKG_VERSION="c4e6ff29c7371c42653edce152d50d18066a4ae7"
PKG_SHA256="4c512c891fa4a1e4326632956b60a96eca3d1341f862ae10c1ef2c98676e4c4b"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/crazycat69/szap-s2"
PKG_URL="https://github.com/crazycat69/szap-s2/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="szap-s2 is a simple zapping tool for the Linux DVB S2 API"
PKG_BUILD_FLAGS="-sysroot"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  make install BIND=${INSTALL}/usr/bin
}
