# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="commander-genius"
PKG_VERSION="4d7ce901f18e8acd46c52723c688356deefd905c"
PKG_ARCH="arm"
PKG_LICENSE="GPLv2"
PKG_SITE="https://gitlab.com/Dringgstein/Commander-Genius"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="libretro"
PKG_SHORTDESC="Modern Interpreter for the Commander Keen Games (Vorticon and Galaxy Games)"
PKG_TOOLCHAIN="cmake"
GET_HANDLER_SUPPORT="git"


makeinstall_target() {
mkdir -p $INSTALL/usr/config/emuelec/config/CommanderGenius
cp -rf $PKG_BUILD/vfsroot/* $INSTALL/usr/config/emuelec/config/CommanderGenius/

mkdir -p $INSTALL/usr/config/emuelec/bin
cp -rf $PKG_BUILD/.${TARGET_NAME}/src/CGeniusExe $INSTALL/usr/config/emuelec/bin
}
