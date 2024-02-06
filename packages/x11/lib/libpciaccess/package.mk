# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libpciaccess"
PKG_VERSION="0.18"
PKG_SHA256="5461b0257d495254346f52a9c329b44b346262663675d3fecdb204a7e7c262a9"
PKG_LICENSE="OSS"
PKG_SITE="https://freedesktop.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros zlib"
PKG_LONGDESC="X.org libpciaccess library."

PKG_MESON_OPTS_TARGET="-Dpci-ids=/usr/share \
                       -Dzlib=enabled"

pre_configure_target() {
  CFLAGS+=" -D_LARGEFILE64_SOURCE"
}
