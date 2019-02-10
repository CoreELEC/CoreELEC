# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="media_tree_aml"
PKG_VERSION="bbd47b0a4e654b4702260b7c57caab25ee42fdf1"
PKG_SHA256="043dd59ca0e7eb936a9c2a73239cc26e7b265ecb56708f029886caabd6c44b32"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC"
PKG_URL="https://github.com/CoreELEC/media_tree_aml/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="Source of Linux Kernel amlogic drivers to build with media_build."
PKG_TOOLCHAIN="manual"


unpack() {
  mkdir -p $PKG_BUILD/
  tar -xf $SOURCES/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.gz -C $PKG_BUILD/../
}
