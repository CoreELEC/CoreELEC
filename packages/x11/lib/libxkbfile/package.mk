# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libxkbfile"
PKG_VERSION="1.1.3"
PKG_SHA256="a9b63eea997abb9ee6a8b4fbb515831c841f471af845a09de443b28003874bec"
PKG_LICENSE="OSS"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libX11"
PKG_LONGDESC="Libxkbfile provides an interface to read and manipulate description files for XKB, the X11 keyboard configuration extension."

PKG_MESON_OPTS_TARGET="-Ddefault_library=static"
