# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXxf86vm"
PKG_VERSION="1.1.7"
PKG_SHA256="ae50c0f669e0af5a67cc4cd0f54f21d64a64d2660af883e80e95d3fe51b945d8"
PKG_LICENSE="X11"
PKG_SITE="http://www.X.org"
PKG_URL="http://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libX11 libXext"
PKG_LONGDESC="The libxxf86vm provides an interface to the server extension XFree86-VidModeExtension."
PKG_BUILD_FLAGS="+pic"

PKG_MESON_OPTS_TARGET="-Ddefault_library=static"
