# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present CoreELEC (https://coreelec.org)

PKG_NAME="media_tree_aml"
PKG_VERSION="e64ee9b314759b70c5f4ee762a6e2ff2768d29a3"
PKG_SHA256="91b17c00b2b15cb0ffe70f2a4b81beb263fc5d1653c3b06c12984a6a0c45caa3"
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
