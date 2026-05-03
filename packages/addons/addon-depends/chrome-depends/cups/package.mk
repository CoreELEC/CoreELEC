# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017 Escalade
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cups"
PKG_VERSION="2.4.19"
PKG_SHA256="89fa632529d3e6a7cb9787ef29e39c4997be0323cc936e05cdb4c79387a35e8e"
PKG_LICENSE="GPL"
PKG_SITE="http://www.cups.org"
PKG_URL="https://github.com/openprinting/cups/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain gnutls zlib"
PKG_LONGDESC="CUPS printing system."
PKG_BUILD_FLAGS="+pic -sysroot"

PKG_CONFIGURE_OPTS_TARGET="--libdir=/usr/lib \
                           --disable-gssapi \
                           --with-dnssd=no \
                           --with-tls=gnutls
                           --disable-unit-tests"

pre_configure_target() {
  cd ..
  rm -rf .${TARGET_NAME}
}

makeinstall_target() {
  make BUILDROOT="${INSTALL}" install
}
