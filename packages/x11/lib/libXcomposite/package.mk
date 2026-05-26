# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libXcomposite"
PKG_VERSION="0.4.7"
PKG_SHA256="8bdf310967f484503fa51714cf97bff0723d9b673e0eecbf92b3f97c060c8ccb"
PKG_LICENSE="MIT"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros libXfixes libXext libX11"
PKG_LONGDESC="X Composite Library"
PKG_BUILD_FLAGS="+pic"
