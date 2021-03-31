# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="sdlpop"
PKG_VERSION="42e006818d82467e76cc928d32b19f43ae96c7fa"
PKG_SHA256="750a32fc1200dab8a737acce5ff912df6d630a3d0388071b7bf5950866f49245"
PKG_ARCH="any"
PKG_LICENSE="GPL3"
PKG_SITE="https://github.com/NagyD/SDLPoP"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git SDL2_image"
PKG_SHORTDESC="Prince of Persia SDL"
PKG_LONGDESC="An open-source port of Prince of Persia, based on the disassembly of the DOS version."
PKG_TOOLCHAIN="make"

pre_configure_target() {
sed -i "s/start_fullscreen = false/start_fullscreen = true/" "$PKG_BUILD/SDLPoP.ini"
sed -i "s/enable_info_screen = true;/enable_info_screen = false;/" "$PKG_BUILD/SDLPoP.ini"
}

make_target() {
	cd $PKG_BUILD/src/
	mkdir -p build
	cd build/
	cmake ..
	make CC=cc
}

makeinstall_target() {
	mkdir -p $INSTALL/usr/config/emuelec/configs/SDLPoP
	mkdir -p $INSTALL/usr/bin
	cp -r $PKG_BUILD/* $INSTALL/usr/config/emuelec/configs/SDLPoP/
	mv "$INSTALL/usr/config/emuelec/configs/SDLPoP/prince" "$INSTALL/usr/bin/"
	rm -rf $INSTALL/usr/config/emuelec/configs/SDLPoP/src
	rm -rf $INSTALL/usr/config/emuelec/configs/SDLPoP/.gitignore
	rm -rf $INSTALL/usr/config/emuelec/configs/SDLPoP/.editorconfig
}
