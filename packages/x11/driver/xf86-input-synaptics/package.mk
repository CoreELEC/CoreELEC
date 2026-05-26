# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xf86-input-synaptics"
PKG_VERSION="1.10.0"
PKG_SHA256="e0c26adb068edd0869f87a87f5e9127922d61c0265d7692a247a91a5cc1bb5c2"
PKG_LICENSE="MIT"
PKG_SITE="https://lists.freedesktop.org/mailman/listinfo/xorg"
PKG_URL="https://xorg.freedesktop.org/archive/individual/driver/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain xorg-server libXi libXext libevdev"
PKG_LONGDESC="Synaptics touchpad driver for X.Org."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--with-xorg-module-dir=${XORG_PATH_MODULES}"

post_configure_target() {
  libtool_remove_rpath libtool
}
