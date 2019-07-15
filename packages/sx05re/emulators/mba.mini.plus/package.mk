# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mba.mini.plus"
PKG_VERSION="663228687bec9ef0a4a72cbe170efc166b7357ef"
PKG_SHA256="b25fe171c787680dd8b5ef895a6bd4d33060de313ab2d58b23cda2a4d5de65a7"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="Non-commercial"
PKG_SITE="https://github.com/SumavisionQ5/MBA.mini.Plus-libretro"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="M.B.A = MAME's skeleton + FBA's romsets"
PKG_LONGDESC="M.B.A-mini from MAME2010-libretro (https://github.com/libretro/mame2010-libretro) after the codes is streamlined, only CPS 1/2, NEOGEO, IREM M92 machines && roms is supported."

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"

pre_configure_target() {
  PKG_MAKE_OPTS_TARGET="platform=armv CC=$CC LD=$CC"

  sed -i -e "s|uname -a|echo armv|" \
         -e "s|uname -m|echo armv|" \
         -e "s|LIBS = |LIBS = -lm|g" \
    makefile
}

makeinstall_target() {
 mkdir -p $INSTALL/usr/lib/libretro
 cp mba_mini_libretro.so $INSTALL/usr/lib/libretro/mba_mini_libretro.so
}
