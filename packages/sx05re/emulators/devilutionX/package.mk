# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="devilutionX"
PKG_VERSION="58e424412a278ecc2d5a04f3019996fbf1466642"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="unlicense"
PKG_SITE="https://github.com/diasurgical/devilutionX"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain SDL2-git SDL2_mixer SDL2_ttf"
PKG_LONGDESC="Diablo build for modern operating systems "
PKG_TOOLCHAIN="cmake-make"
GET_HANDLER_SUPPORT="git"

PKG_CMAKE_OPTS_TARGET=" -DNONET=ON -DCMAKE_BUILD_TYPE="Release" -DASAN=OFF -DUBSAN=OFF -DDEBUG=OFF -DLTO=ON -DDIST=OFF -DFASTER=OFF"

makeinstall_target() { 
mkdir -p $INSTALL/usr/config/emuelec/bin
cp -rf $PKG_BUILD/.$TARGET_NAME/devilutionx $INSTALL/usr/config/emuelec/bin
cp -rf $PKG_BUILD/Packaging/resources/CharisSILB.ttf $PKG_BUILD/.$TARGET_NAME/devilutionx $INSTALL/usr/config/emuelec/bin
}
