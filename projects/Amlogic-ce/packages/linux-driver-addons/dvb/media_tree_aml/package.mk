# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="media_tree_aml"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC"
PKG_DEPENDS_TARGET="toolchain"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="Source of Linux Kernel amlogic drivers to build with media_build."
PKG_TOOLCHAIN="manual"

case "${LINUX}" in
  amlogic-3.14)
    PKG_VERSION="f9a4b158183866c589ee8c06ab740f41aa08fa66"
    PKG_SHA256="ff629f20b39749e6523571409c74c38a27ba0101b0ffe2a1ecea3e4dafea6dd2"
    PKG_URL="https://github.com/CoreELEC/media_tree_aml/archive/${PKG_VERSION}.tar.gz"
    ;;
  amlogic-4.9)
    PKG_VERSION="5de4194a41241aea6c821aa8c39465859aaaf5f6"
    PKG_SHA256="a11f17952c9826f4e43612fdf45924316488d66d4378791fa4ce2ce3ca1e8cad"
    PKG_URL="https://github.com/CoreELEC/media_tree_aml/archive/${PKG_VERSION}.tar.gz"
    ;;
esac

unpack() {
  mkdir -p ${PKG_BUILD}/
  tar -xf ${SOURCES}/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.gz -C ${PKG_BUILD}/../
}
