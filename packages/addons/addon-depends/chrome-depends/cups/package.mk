# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017 Escalade
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cups"
PKG_VERSION="2.4.10"
PKG_SHA256="f51b9edd631db1830b967101b51f0045c8c239ae799dff89f0399f3e47a95c02"
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
