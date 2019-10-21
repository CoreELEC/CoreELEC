# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="jslisten"
PKG_VERSION="039084cb2c006586f87a4f407da4762977065104"
PKG_SHA256="03a3f8ae41d174d6f326a32f6a9f0f0bcbf04d26e2d26f472b90a1621015bdf6"
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
