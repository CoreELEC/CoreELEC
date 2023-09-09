# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="font-util"
PKG_VERSION="1.4.1"
PKG_SHA256="5c9f64123c194b150fee89049991687386e6ff36ef2af7b80ba53efaf368cc95"
PKG_LICENSE="OSS"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/font/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros"
PKG_DEPENDS_HOST="util-macros"
PKG_LONGDESC="X.org font utilities."

PKG_CONFIGURE_OPTS_TARGET="--with-fontrootdir=/usr/share/fonts \
                           --with-mapdir=/usr/share/fonts/util"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
}
