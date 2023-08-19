# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xcb-proto"
PKG_VERSION="1.16.0"
PKG_SHA256="a75a1848ad2a89a82d841a51be56ce988ff3c63a8d6bf4383ae3219d8d915119"
PKG_LICENSE="OSS"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/proto/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros Python3:host"
PKG_LONGDESC="X C-language Bindings protocol headers."

post_makeinstall_target() {
  python_remove_source
}
