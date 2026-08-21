# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-common"
PKG_VERSION="879c8d507b0b52e77e27d759239c2b5df1e26dfd"
PKG_SHA256="b464f796ff9752f9fb53c4ced9a0346e428fdbf7786032abac79d106d7170fcd"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/libretro/libretro-common"
PKG_URL="https://github.com/libretro/libretro-common/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Reusable coding blocks useful for libretro core and frontend development"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p "${SYSROOT_PREFIX}/usr/include/${PKG_NAME}"
  cp -pR ${PKG_BUILD}/include/* "${SYSROOT_PREFIX}/usr/include/${PKG_NAME}/"
}
