# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="opentyrian"
PKG_VERSION="84b820f852f3f6b812b4d00d6b3906adbbf3bbdb"
PKG_SHA256="7429cc8e3468e3462b886cb99fe6cc0f5d232c193b68a94dc427493107c30dec"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL2"
PKG_SITE="https://github.com/opentyrian/opentyrian"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git"
PKG_LONGDESC="An open-source port of the DOS shoot-em-up Tyrian."
PKG_TOOLCHAIN="make"

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  cp opentyrian $INSTALL/usr/bin
  
  mkdir -p $INSTALL/usr/config/opentyrian
  cp -r $PKG_DIR/config/* $INSTALL/usr/config/opentyrian
}
