# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cxxtools"
PKG_VERSION="4b3e696a201755ae83bbf0a45d1c25f176463cf8"
PKG_SHA256="9a532b86512365759050554dcab923a5620ab4dce3176c4ed31626213f5bfd9a"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://github.com/maekitalo/cxxtools"
PKG_URL="${PKG_SITE}/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="toolchain:host openssl:host"
PKG_DEPENDS_TARGET="toolchain cxxtools:host openssl"
PKG_LONGDESC="Cxxtools is a collection of general-purpose C++ components."
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_HOST="-DBUILD_SHARED_LIBS=ON -DBUILD_DEMOS=OFF -DBUILD_TESTS=OFF"
PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF -DBUILD_DEMOS=OFF -DBUILD_TESTS=OFF"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
}
