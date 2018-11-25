# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present CoreELEC (https://coreelec.org)

PKG_NAME="media_tree_aml"
PKG_VERSION="619ba251386da72897262a39878c17804905a71d"
PKG_SHA256="87a218c768e9377da857d1631325c6a1a4d65eeea6ccb9946268f4619f0e9806"
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
