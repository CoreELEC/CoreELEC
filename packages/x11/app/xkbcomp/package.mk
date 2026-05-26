# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xkbcomp"
PKG_VERSION="1.5.0"
PKG_SHA256="2ac31f26600776db6d9cd79b3fcd272263faebac7eb85fb2f33c7141b8486060"
PKG_LICENSE="MIT"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/app/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libX11 libxkbfile"
PKG_LONGDESC="The xkbcomp keymap compiler converts a description of an XKB keymap into one of several output formats."

PKG_MESON_OPTS_TARGET="-Dxkb-config-root=${XORG_PATH_XKB}"
