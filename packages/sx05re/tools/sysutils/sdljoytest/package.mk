# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="sdljoytest"
PKG_VERSION="c316d9e87a102e1d42882fc862f7953e578ae257"
PKG_SHA256="b5812d24990eb4092ce3b46dc5b1670ba1d80bfee6c52a5ae4eb24abbf48359d"
PKG_LICENSE="OSS"
PKG_SITE="https://github.com/Wintermute0110/sdljoytest"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git"
PKG_LONGDESC="Test joystick with SDL2 in Linux"
PKG_TOOLCHAIN="make"

pre_configure_target() {
sed -i "s|gcc|${CC}|" Makefile
}

makeinstall_target() {
mkdir -p $INSTALL/usr/bin
cp -rf test_gamepad_SDL2 $INSTALL/usr/bin/sdljoytest
cp -rf map_gamepad_SDL2 $INSTALL/usr/bin/sdljoymap
}
