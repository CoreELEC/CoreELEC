# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="waf"
PKG_VERSION="2.1.9"
PKG_SHA256="4a47a431b86d5c42fa23b8474aaf752384dcaed65fe9e6aa1f518c103d38a7d1"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://waf.io"
PKG_URL="https://waf.io/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_LONGDESC="The Waf build system"
PKG_TOOLCHAIN="manual"

makeinstall_host() {
  cp -pf ${PKG_BUILD}/waf ${TOOLCHAIN}/bin/
}
