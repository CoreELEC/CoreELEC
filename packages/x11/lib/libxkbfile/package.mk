# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libxkbfile"
PKG_VERSION="1.2.0"
PKG_SHA256="7f71884e5faf56fb0e823f3848599cf9b5a9afce51c90982baeb64f635233ebf"
PKG_LICENSE="MIT"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libX11"
PKG_LONGDESC="Libxkbfile provides an interface to read and manipulate description files for XKB, the X11 keyboard configuration extension."

PKG_MESON_OPTS_TARGET="-Ddefault_library=static"
