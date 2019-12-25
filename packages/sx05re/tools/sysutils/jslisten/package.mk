# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="jslisten"
PKG_VERSION="0d137b8e97e2a0f4c7e6383f85db19906b479acb"
PKG_SHA256="39de471269fa620e24d7cfd91211ced6f5d4925da84ee6090749d88e16cb6bbf"
PKG_LICENSE="GPL3"
PKG_SITE="https://github.com/shantigilbert/jslisten"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_GIT_CLONE_BRANCH="EmuELEC"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="listen to gamepad inputs and trigger a command, cloned from https://github.com/workinghard/jslisten"
PKG_TOOLCHAIN="make"

make_target() {
mkdir bin
make 
}

makeinstall_target() {
mkdir -p $INSTALL/usr/config/emuelec/bin
cp bin/jslisten $INSTALL/usr/config/emuelec/bin
} 
