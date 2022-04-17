# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="fmsx-libretro"
PKG_VERSION="11fa9f3c08cde567394c41320ca76798c2c64670"
PKG_SHA256="0964183b3ac858ce1d78a31b3e2cd429841c371ef56bf3add658903b0c6d017e"
PKG_ARCH="any"
PKG_LICENSE="OPEN/NON-COMMERCIAL"
PKG_SITE="https://github.com/libretro/fmsx-libretro"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SHORTDESC="Port of fMSX to the libretro API. "
PKG_TOOLCHAIN="make"

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp fmsx_libretro.so $INSTALL/usr/lib/libretro/
}
