# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libICE"
PKG_VERSION="1.1.2"
PKG_SHA256="974e4ed414225eb3c716985df9709f4da8d22a67a2890066bc6dfc89ad298625"
PKG_LICENSE="MIT"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros xtrans"
PKG_LONGDESC="X Inter-Client Exchange (ICE) protocol library."
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-ipv6 \
                           --without-xmlto"
