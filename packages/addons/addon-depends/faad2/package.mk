# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2014 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="faad2"
PKG_VERSION="2.11.1"
PKG_SHA256="72dbc0494de9ee38d240f670eccf2b10ef715fd0508c305532ca3def3225bb06"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/knik0/faad2/"
PKG_URL="https://github.com/knik0/faad2/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="An MPEG-4 AAC decoder."

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF"

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
}
