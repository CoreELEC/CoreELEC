# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="waf"
PKG_VERSION="2.0.26"
PKG_SHA256="c33d19c1bdfae1b078edaef2fab19b1bc734294edd4cc005d4881e9d53ed219c"
PKG_LICENSE="MIT"
PKG_SITE="https://waf.io"
PKG_URL="https://waf.io/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_LONGDESC="The Waf build system"
PKG_TOOLCHAIN="manual"

makeinstall_host() {
  cp -pf ${PKG_BUILD}/waf ${TOOLCHAIN}/bin/
}
