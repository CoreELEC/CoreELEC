# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2010-2011 Roman Weber (roman@openelec.tv)
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="attr"
PKG_VERSION="2.5.2"
PKG_SHA256="39bf67452fa41d0948c2197601053f48b3d78a029389734332a6309a680c6c87"
PKG_LICENSE="GPL"
PKG_SITE="https://savannah.nongnu.org/projects/attr"
PKG_URL="http://download.savannah.nongnu.org/releases/attr/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="autotools:host gcc:host"
PKG_LONGDESC="Extended Attributes Of Filesystem Objects."
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="OPTIMIZER= \
                           CONFIG_SHELL=/bin/bash \
                           INSTALL_USER=root INSTALL_GROUP=root \
                           --disable-shared --enable-static"

if build_with_debug; then
  PKG_CONFIGURE_OPTS_TARGET+=" DEBUG=-DDEBUG"
else
  PKG_CONFIGURE_OPTS_TARGET+=" DEBUG=-DNDEBUG"
fi

post_makeinstall_target() {
  safe_remove ${INSTALL}/etc
  safe_remove ${INSTALL}/usr/bin
}
