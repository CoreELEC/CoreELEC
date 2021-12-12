# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="potator"
PKG_VERSION="3e3bfd787306d5a074a91bfd9baee2f9cc3b82f3"
PKG_SHA256="95952073bca7db2fce3747059dea1d809246026928d8785a5ba11edd6e5c6214"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="The Unlicense"
PKG_SITE="https://github.com/libretro/potator"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_LONGDESC="A Watara Supervision Emulator based on Normmatt version "
PKG_TOOLCHAIN="make"

make_target() {
cd $PKG_BUILD/platform/libretro
make
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp $PKG_BUILD/platform/libretro/potator_libretro.so $INSTALL/usr/lib/libretro/potator_libretro.so
}
