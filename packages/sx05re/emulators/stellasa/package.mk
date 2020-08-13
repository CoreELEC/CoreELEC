# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="stellasa"
PKG_VERSION="7964c93a2fc3f0690f03ed39c6f7c2a9293e3f5f"
PKG_SHA256="3de1b3479a64140b163c5f55a2ed27cd1642ae76b0fe5845a2e77451fac86a57"
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
