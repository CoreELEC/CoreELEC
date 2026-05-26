# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libSM"
PKG_VERSION="1.2.6"
PKG_SHA256="be7c0abdb15cbfd29ac62573c1c82e877f9d4047ad15321e7ea97d1e43d835be"
PKG_LICENSE="MIT"
PKG_SITE="http://www.X.org"
PKG_URL="http://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros util-linux libICE"
PKG_LONGDESC="This package provides the main interface to the X11 Session Management library."
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --with-libuuid \
                           --without-xmlto \
                           --without-fop"
