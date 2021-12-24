# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="devilutionX"
PKG_VERSION="aee8ed3951af2cb635a7e2ce3b6ebe0ce782ee67"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="unlicense"
PKG_SITE="https://github.com/diasurgical/devilutionX"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain SDL2 SDL2_mixer SDL2_ttf"
PKG_LONGDESC="Diablo build for modern operating systems "
GET_HANDLER_SUPPORT="git"
PKG_BUILD_FLAGS="-lto"

pre_configure_target() {
PKG_CMAKE_OPTS_TARGET=" -DBINARY_RELEASE=1 -DCMAKE_BUILD_TYPE="Release" -DDEBUG=OFF -DPREFILL_PLAYER_NAME=ON -DDEVILUTIONX_SYSTEM_LIBSODIUM=OFF -DMO_LANG_DIR=\"/emuelec/configs/devilution/langs/\""
sed -i "s|;-static-libstdc++>|;-lstdc++>|" $PKG_BUILD/CMakeLists.txt

sed -i "s|assets/|assets_dvx/|" $PKG_BUILD/Source/utils/paths.cpp
}

makeinstall_target() { 
mkdir -p $INSTALL/usr/bin/assets_dvx
cp -rf $PKG_BUILD/.$TARGET_NAME/devilutionx $INSTALL/usr/bin
cp -rf $PKG_DIR/scripts/* $INSTALL/usr/bin
cp -rf $PKG_BUILD/.$TARGET_NAME/assets/* $INSTALL/usr/bin/assets_dvx/

mkdir -p ${INSTALL}/usr/config/emuelec/configs/devilution/langs
mv $INSTALL/usr/bin/assets_dvx/*.gmo ${INSTALL}/usr/config/emuelec/configs/devilution/langs

}
