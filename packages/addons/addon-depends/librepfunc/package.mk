# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="librepfunc"
PKG_VERSION="1.8.1"
PKG_SHA256="ea0c68fd698aa4660fe870906ce2128e7925135d5abaaf19cd80f5054d177148"
PKG_LICENSE="GPL-2.0"
PKG_SITE="https://github.com/wirbel-at-vdr-portal/librepfunc"
PKG_URL="https://github.com/wirbel-at-vdr-portal/librepfunc/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="librepfunc - a collection of functions, classes used by wirbel-at-vdr-portal"
PKG_BUILD_FLAGS="-sysroot"

pre_configure_target() {
  sed -i -e 's/^install:/install-shared:/' -e 's/^install-static:/install:/' Makefile
}
