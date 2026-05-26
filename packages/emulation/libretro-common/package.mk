# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-common"
PKG_VERSION="5b5a830baa6c72452c1f1d4ac3e2fdd04bfbd267"
PKG_SHA256="007728cbf8940cb53f867190b77080bc654b285b77e2a79c1404f78ea5e9e8b4"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/libretro/libretro-common"
PKG_URL="https://github.com/libretro/libretro-common/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Reusable coding blocks useful for libretro core and frontend development"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p "${SYSROOT_PREFIX}/usr/include/${PKG_NAME}"
  cp -pR ${PKG_BUILD}/include/* "${SYSROOT_PREFIX}/usr/include/${PKG_NAME}/"
}
