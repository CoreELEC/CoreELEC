# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present CoreELEC (https://coreelec.org)

PKG_NAME="media_tree_aml"
PKG_VERSION="bc8a4780a550690b92aeb65b049348c537d3593c"
PKG_SHA256="fa0438fb7a93b080150508b8d5bb72422a0e89ba1a9c0313ffd9298e21cf0b07"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_URL="https://github.com/CoreELEC/media_tree_aml/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="driver"
PKG_LONGDESC="Source of Linux Kernel amlogic drivers to build with media_build."
PKG_TOOLCHAIN="manual"

unpack() {
  mkdir -p $PKG_BUILD/
  tar -xf $SOURCES/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.gz -C $PKG_BUILD/../
}
