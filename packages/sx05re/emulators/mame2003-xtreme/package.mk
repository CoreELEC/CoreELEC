# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mame2003-xtreme"
PKG_VERSION="ace37581d0fe0148cc0fd2857d891f5dd4107e91"
PKG_SHA256="fe9fa1a5f44ac6c93439eea00a0ee20530c19d845cdec3c35f3ed9f481203682"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="MAME"
PKG_SITE="https://github.com/KMFDManic/mame2003-xtreme"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Updated 2018 version of MAME (0.78) for libretro, with added game support, and optimized for performance and speed on the Mini Classics. "
PKG_TOOLCHAIN="make"

pre_configure_target() {
  cd ${PKG_BUILD}
  export SYSROOT_PREFIX=${SYSROOT_PREFIX}

  case ${PROJECT} in
    Amlogic-ng)
        PKG_MAKE_OPTS_TARGET+=" platform=AMLG12B"
      ;;
    Amlogic)
        PKG_MAKE_OPTS_TARGET+=" platform=AMLGX"
      ;;
  esac
  PKG_MAKE_OPTS_TARGET+=" ARCH=\"\" CC=\"$CC\" NATIVE_CC=\"$CC\" LD=\"$CC\""
  
  # PKG_MAKE_OPTS_TARGET=" platform=rpi2 ARCH=\"\" CC=\"$CC\" NATIVE_CC=\"$CC\" LD=\"$CC\""
  
 }

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp mame2003_libretro.so $INSTALL/usr/lib/libretro/km_mame2003_xtreme_libretro.so
  cp km_mame2003_xtreme_libretro.info $INSTALL/usr/lib/libretro/km_mame2003_xtreme_libretro.info
  
}
