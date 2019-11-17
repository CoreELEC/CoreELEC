# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="scummvmsa"
PKG_VERSION="cc2973f04f5e46eb1c07ee5ff1e400cf9a85e64e"
PKG_SHA256="49f172ffa0e4e0875c8899b502460bc761ea910f25b60c456e4ff6842350b2df"
PKG_REV="1"
PKG_LICENSE="GPL2"
PKG_SITE="https://github.com/scummvm/scummvm"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git SDL2_net freetype fluidsynth-git"
PKG_SHORTDESC="Script Creation Utility for Maniac Mansion Virtual Machine"
PKG_LONGDESC="ScummVM is a program which allows you to run certain classic graphical point-and-click adventure games, provided you already have their data files."

pre_configure_target() { 
sed -i "s|sdl-config|sdl2-config|g" $PKG_BUILD/configure
TARGET_CONFIGURE_OPTS="--host=${TARGET_NAME} --backend=sdl --enable-optimizations --opengl-mode=gles2 --with-sdl-prefix=${SYSROOT_PREFIX}/usr/bin"
}

post_makeinstall_target() {
mkdir -p $INSTALL/usr/config/scummvm/extra 
	cp -rf $PKG_DIR/config/* $INSTALL/usr/config/scummvm/
	cp -rf $PKG_BUILD/backends/vkeybd/packs/*.zip $INSTALL/usr/config/scummvm/extra

mv $INSTALL/usr/local/bin $INSTALL/usr/
	cp -rf $PKG_DIR/bin/* $INSTALL/usr/bin
	
for i in appdata applications doc icons man; do
    rm -rf "$INSTALL/usr/local/share/$i"
  done

 
}

