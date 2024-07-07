# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="encodings"
PKG_VERSION="1.1.0"
PKG_SHA256="9ff13c621756cfa12e95f32ba48a5b23839e8f577d0048beda66c67dab4de975"
PKG_LICENSE="OSS"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/font/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros font-util:host"
PKG_LONGDESC="X font encoding meta files."

PKG_MESON_OPTS_TARGET="-Dgzip-small-encodings=true \
                       -Dgzip-large-encodings=true \
                       -Dfontrootdir=/usr/share/fonts"
