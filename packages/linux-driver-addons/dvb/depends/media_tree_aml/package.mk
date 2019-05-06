# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="media_tree_aml"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC"
PKG_DEPENDS_TARGET="toolchain"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="Source of Linux Kernel amlogic drivers to build with media_build."
PKG_TOOLCHAIN="manual"

case "$LINUX" in
  amlogic-3.14)
    PKG_VERSION="bbd47b0a4e654b4702260b7c57caab25ee42fdf1"
    PKG_SHA256="043dd59ca0e7eb936a9c2a73239cc26e7b265ecb56708f029886caabd6c44b32"
    PKG_URL="https://github.com/CoreELEC/media_tree_aml/archive/${PKG_VERSION}.tar.gz"
    ;;
  amlogic-4.9)
    PKG_VERSION="0f6cd09b6ad228b8a5517e7879ade76a72eec5c2"
    PKG_SHA256="29f27176cc0aff7a568c623ce19d0ac0757c7472b533a9d4253d115c75fe5c6a"
    PKG_URL="https://github.com/CoreELEC/media_tree_aml/archive/${PKG_VERSION}.tar.gz"
    ;;
esac

unpack() {
  mkdir -p $PKG_BUILD/
  tar -xf $SOURCES/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.gz -C $PKG_BUILD/../
}
