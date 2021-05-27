# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="supermariowar"
PKG_VERSION="17b5edf165cb964ef245e57fe5b59451e683c569"
PKG_ARCH="any"
PKG_SITE="https://github.com/mmatyas/supermariowar"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain SDL2-git SDL2_image SDL2_mixer"
PKG_SHORTDESC="A fan-made multiplayer Super Mario Bros. style deathmatch game "
PKG_TOOLCHAIN="cmake"

PKG_CMAKE_OPTS_TARGET=" -DUSE_PNG_SAVE=ON"

makeinstall_target() {
mkdir -p $INSTALL/usr/bin
cp $PKG_BUILD/.${TARGET_NAME}/Binaries/Release/* $INSTALL/usr/bin
cp $PKG_DIR/scripts/* $INSTALL/usr/bin
} 
