# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="residualvm"
PKG_VERSION="626c55780b605030e2fb308f7de0947e840ace09"
PKG_SHA256="ccd2140acab70e6645e804bbb15fb4058e27ac08a9abcda2ba2872d1e01b5c07"
PKG_REV="1"
PKG_LICENSE="GPL2"
PKG_SITE="https://github.com/residualvm/residualvm"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git SDL2_net freetype fluidsynth-git libjpeg-turbo libmad"
PKG_SHORTDESC="ResidualVM: A 3D game interpreter"
PKG_LONGDESC="ResidualVM is a game engine reimplementation that allows you to play 3D adventure games such as Grim Fandango, Escape from Monkey Island, Myst III and The Longest Journey."
PKG_TOOLCHAIN="configure"

pre_configure_target() { 
sed -i "s|sdl-config|sdl2-config|g" $PKG_BUILD/configure
TARGET_CONFIGURE_OPTS="--host=${TARGET_NAME} --backend=sdl --enable-optimizations --force-opengles2 --with-sdl-prefix=${SYSROOT_PREFIX}/usr/bin"
}

post_makeinstall_target() { 
for i in applications icons doc man; do
    rm -rf "$INSTALL/usr/local/share/$i"
  done

mkdir -p $INSTALL/usr/config/residualvm/extra
	cp -rf $PKG_DIR/config/* $INSTALL/usr/config/residualvm/
	cp -rf $PKG_BUILD/backends/vkeybd/packs/*.zip $INSTALL/usr/config/residualvm/extra
	mv  $INSTALL/usr/local/bin $INSTALL/usr/
	cp $PKG_DIR/bin/* $INSTALL/usr/bin/
}
