# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mba.mini.plus"
PKG_VERSION="8b8f623ef241c7aa21f2f775eb1b27364e0bd556"
PKG_SHA256="55240dc16b483a4ff410c84ad209fc6768bd0dafe0a97bb1853db07adfb303b3"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="Non-commercial"
PKG_SITE="https://github.com/SumavisionQ5/MBA.mini.Plus-libretro"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="emuelec"
PKG_SHORTDESC="M.B.A = MAME's skeleton + FBA's romsets"
PKG_LONGDESC="M.B.A-mini from MAME2010-libretro (https://github.com/libretro/mame2010-libretro) after the codes is streamlined, only CPS 1/2, NEOGEO, IREM M92 machines && roms is supported."
PKG_TOOLCHAIN="make"


pre_configure_target() {
  
  if [ ${PROJECT} = "Amlogic-ng" ]; then
	PKG_MAKE_OPTS_TARGET="platform=AMLG12B"
  elif [ "${PROJECT}" = "Amlogic" ]; then
	PKG_MAKE_OPTS_TARGET="platform=AMLGX"
  fi
  
  PKG_MAKE_OPTS_TARGET+=" CC=$CC LD=$CC"
  
  sed -i -e "s|uname -a|echo armv|" \
         -e "s|uname -m|echo armv|" \
         -e "s|LIBS = -lm|LIBS = |g" \
         -e "s|LIBS = |LIBS = -lm|g" \
    makefile
}

makeinstall_target() {
 mkdir -p $INSTALL/usr/lib/libretro
 cp mba_mini_libretro.so $INSTALL/usr/lib/libretro/mba_mini_libretro.so
}
