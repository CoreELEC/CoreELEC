# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="jslisten"
PKG_VERSION="5e09d97963198051c8ccfdd56e6f99e9d2b03de8"
PKG_SHA256="90ddc5a05a76a237736eded42d14a50851f82db164e10f64e1e84f9521d44304"
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
