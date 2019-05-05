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
    PKG_VERSION="887df21bb3f9e36c24e31fd38d4a98c9c0d93a92"
    PKG_SHA256="c3957f90f5bf8bba74ca7498863e457cf48b93995fa074af8a34fa0b3fbc0a7c"
    PKG_URL="https://github.com/CoreELEC/media_tree_aml/archive/${PKG_VERSION}.tar.gz"
    ;;
esac

unpack() {
  mkdir -p $PKG_BUILD/
  tar -xf $SOURCES/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.gz -C $PKG_BUILD/../
}
