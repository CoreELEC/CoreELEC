# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXt"
PKG_VERSION="1.3.1"
PKG_SHA256="e0a774b33324f4d4c05b199ea45050f87206586d81655f8bef4dba434d931288"
PKG_LICENSE="MIT AND HPND-sell-variant AND MIT-open-group"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libX11 libSM"
PKG_LONGDESC="libXt provides the X Toolkit Intrinsics library, an abstract widget library upon which other toolkits are based."

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --with-gnu-ld \
                           --enable-malloc0returnsnull"

pre_make_target() {
  make -C util CC=${HOST_CC} \
               CFLAGS="${HOST_CFLAGS} " \
               LDFLAGS="${HOST_LDFLAGS}" \
               makestrs
}
