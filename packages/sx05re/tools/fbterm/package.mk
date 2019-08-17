# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="fbterm"
PKG_VERSION="0b59c941f7652d93e02ad94b961f040f937a67d2"
PKG_SHA256="8912fa6b810d2bb75d5cab262fb373af5025a20944b7e9fac7fb8368a630031a"
PKG_LICENSE="GPLv2+"
PKG_SITE="https://github.com/matlinuxer2/fbterm"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain freetype fontconfig"
PKG_LONGDESC=" fbterm is a framebuffer based terminal emulator for linux "
PKG_TOOLCHAIN="configure"

pre_configure_target() {
  cd ..
  rm -rf .$TARGET_NAME
}

makeinstall_target() { 
mkdir -p $INSTALL/usr/bin
cp -rf $PKG_BUILD/src/fbterm $INSTALL/usr/bin
mkdir -p $INSTALL/usr/share/terminfo
cp -rf $PKG_DIR/terminfo/* $INSTALL/usr/share/terminfo/
tic $PKG_BUILD/terminfo/fbterm -o $INSTALL/usr/share/terminfo
# mv $INSTALL/usr/share/terminfo/f/fbterm $INSTALL/usr/share/terminfo/f/linux
# mv $INSTALL/usr/share/terminfo/f $INSTALL/usr/share/terminfo/l
# mkdir -p $INSTALL/usr/share/terminfo/f/
# cp $PKG_BUILD/terminfo/fbterm $INSTALL/usr/share/terminfo/f/
}
