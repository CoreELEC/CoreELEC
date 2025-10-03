# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2024-present Team CoreELEC (https://coreelec.org)

PKG_NAME="AVL6862"
PKG_VERSION="08558dc457443658f2d54077090f5d3d89eb3335"
PKG_SHA256="63c866ef7662afc71c4381d9baba2e97fe3fcb87b6637073b1af42a6556df008"
PKG_REV="1"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC"
PKG_URL="https://github.com/CoreELEC/media_tree_aml/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain media_modules-aml"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="AVL6862 driver for Amlogic DVB Frontend"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_IS_KERNEL_PKG="yes"
PKG_ADDON_IS_STANDALONE="yes"
PKG_ADDON_NAME="AVL6862 driver for Amlogic DVB Frontend"
PKG_ADDON_TYPE="xbmc.service"
PKG_ADDON_VERSION="${ADDON_VERSION}.${PKG_REV}"

make_target() {
  kernel_make -C $(kernel_path) M=${PKG_BUILD} \
    CONFIG_MESON_DVB=m \
    KCFLAGS=-Wno-implicit-fallthrough \
    KBUILD_EXTRA_SYMBOLS=$(get_build_dir media_modules-aml)/drivers/Module.symvers
}

makeinstall_target() {
  install_driver_addon_files "${PKG_BUILD}"
}
