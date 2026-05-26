# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tune-s2"
PKG_VERSION="60cc4aaa70b646d38f2e40251860375283c44816"
PKG_SHA256="f2e7546c70d9b29abc2e9fcfd2f0d3f960c00112e9f7143962f7ff99da929b08"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/crazycat69/tune-s2"
PKG_URL="https://github.com/crazycat69/tune-s2/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="tune-s2 is a small linux app to be able to tune a dvb devices"
PKG_BUILD_FLAGS="-sysroot"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  make install BIND=${INSTALL}/usr/bin
}
