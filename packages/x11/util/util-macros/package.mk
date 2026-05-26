# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="util-macros"
PKG_VERSION="1.20.2"
PKG_SHA256="9ac269eba24f672d7d7b3574e4be5f333d13f04a7712303b1821b2a51ac82e8e"
PKG_LICENSE="MIT"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/util/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="X.org autoconf utilities such as M4 macros."
PKG_BUILD_FLAGS="-cfg-libs"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr
}
