# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="stellasa"
PKG_VERSION="64dc9dde8665280e6ac0fcd6a5477ec2ade7bf9f"
PKG_SHA256="c493b53161007ba8d6c132689e5240e76e3c7374727c5eadeb26f67d59ce6fa6"
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
