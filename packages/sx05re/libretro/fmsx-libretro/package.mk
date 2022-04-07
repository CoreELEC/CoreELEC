# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="fmsx-libretro"
PKG_VERSION="f9ea9eacd49297783c216d147dcc1a22465b2749"
PKG_SHA256="0c67bd4034adb0d5fa2ac73e764a57809bae4cf445050040efb35310bf572e45"
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
