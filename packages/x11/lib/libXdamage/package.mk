# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXdamage"
PKG_VERSION="1.1.7"
PKG_SHA256="127067f521d3ee467b97bcb145aeba1078e2454d448e8748eb984d5b397bde24"
PKG_LICENSE="HPND-sell-variant"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libX11 libXfixes"
PKG_LONGDESC="LibXdamage provides an X Window System client interface to the DAMAGE extension to the X protocol."
PKG_BUILD_FLAGS="+pic"
