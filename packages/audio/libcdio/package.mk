# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libcdio"
PKG_VERSION="2.3.0"
PKG_SHA256="53e83d284667535a767fd2d31edad1a6701591960459df373a10f1f21e80a7ed"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://www.gnu.org/software/libcdio/"
PKG_URL="https://github.com/libcdio/libcdio/releases/download/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A CD-ROM reading and control library."
PKG_BUILD_FLAGS="+pic"

# package specific configure options
PKG_CONFIGURE_OPTS_TARGET="--enable-cxx \
                           --disable-cpp-progs \
                           --disable-shared \
                           --enable-static \
                           --enable-joliet \
                           --disable-rpath \
                           --enable-rock \
                           --disable-cddb \
                           --disable-vcd-info \
                           --without-cd-drive \
                           --without-cd-info \
                           --without-cdda-player \
                           --without-cd-read \
                           --without-iso-info \
                           --without-iso-read \
                           --without-libiconv-prefix \
                           --with-gnu-ld"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
}
