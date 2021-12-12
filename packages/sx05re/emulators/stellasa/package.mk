# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="stellasa"
PKG_VERSION="f999bd685af24f1ebc705e146d33b46a8fb0beac"
PKG_SHA256="bdbc7fe4a906f7712f2fd5bffdca1d29da4660eebc14a35268c1780beb74ccdb"
PKG_REV="1"
PKG_LICENSE="GPL2"
PKG_SITE="https://github.com/stella-emu/stella"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SHORTDESC="A multi-platform Atari 2600 Emulator"
PKG_TOOLCHAIN="configure"

pre_configure_target() { 
TARGET_CONFIGURE_OPTS="--host=${TARGET_NAME} --with-sdl-prefix=${SYSROOT_PREFIX}/usr/bin --disable-windowed"
}

make_target() {
cd $PKG_BUILD
mv $PKG_BUILD/.${TARGET_NAME}/* $PKG_BUILD
make 
}

makeinstall_target() {
mkdir -p $INSTALL/usr/bin
cp -rf $PKG_BUILD/stella $INSTALL/usr/bin
cp -rf $PKG_DIR/scripts/* $INSTALL/usr/bin
}
