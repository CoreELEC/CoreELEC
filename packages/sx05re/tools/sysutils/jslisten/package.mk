# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="jslisten"
PKG_VERSION="f08e98533b1c77bd9d250348ffc40c24abe1af32"
PKG_SHA256="d211c26c0815ca77c692b246f02178994bcfbf3917aa39d15ccb2992e1a7e5c4"
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
