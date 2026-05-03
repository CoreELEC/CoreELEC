# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="udpxy"
PKG_VERSION="31d4bcfabaade59d3efdee015df7979febf76bae"
PKG_SHA256="75e5548044a40544c050abfad0dedef8748ede1725d8011d6afc811554b80b92"
PKG_LICENSE="GPLv3"
PKG_SITE="http://www.udpxy.com"
PKG_URL="https://github.com/pcherenkov/udpxy/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A UDP-to-HTTP multicast traffic relay daemon."
PKG_BUILD_FLAGS="-sysroot"

PKG_MAKEINSTALL_OPTS_TARGET="PREFIX=/usr"

post_unpack() {
  mv ${PKG_BUILD}/README ${PKG_BUILD}/README-
  mv ${PKG_BUILD}/chipmunk/* ${PKG_BUILD}
  rmdir ${PKG_BUILD}/chipmunk
}

configure_target() {
  export CFLAGS+=" -Wno-stringop-truncation"
}
