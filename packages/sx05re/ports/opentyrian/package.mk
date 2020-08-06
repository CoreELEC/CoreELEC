# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="opentyrian"
PKG_VERSION="30850004fd90789d7f87cdfcab7d9513c64c136e"
PKG_SHA256="71b22d9857c311ade7834add1f1871a3bc43689b96ba1b898fba63b6914307c5"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL2"
PKG_SITE="https://github.com/opentyrian/opentyrian"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git"
PKG_LONGDESC="An open-source port of the DOS shoot-em-up Tyrian."
PKG_TOOLCHAIN="make"
PKG_GIT_BRANCH="sdl2"

makeinstall_target() {
  mkdir -p $INSTALL/usr/config/emuelec/bin
  cp opentyrian $INSTALL/usr/config/emuelec/bin
  
  mkdir -p $INSTALL/usr/config/opentyrian
  cp -r $PKG_DIR/config/* $INSTALL/usr/config/opentyrian
}
