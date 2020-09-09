# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="dolphinSA"
PKG_VERSION="75b4f70e5eaf50e5c2a05633d2bf91d0b99c25a1"
PKG_SHA256="3953b77280642c1e89659f9a89490cbd263684bb131f5e4cf47d7e9ad89bd8b5"
PKG_ARCH="aarch64"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/dolphin-emu/dolphin"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain libevdev"
PKG_LONGDESC="Dolphin is a GameCube / Wii emulator, allowing you to play games for these two platforms on PC with improvements. "
PKG_BUILD_FLAGS="-lto"

PKG_CMAKE_OPTS_TARGET=" -DDISTRIBUTOR='EmuELEC' -DBUILD_SHARED_LIBS=OFF -DTHREADS_PTHREAD_ARG=OFF -DENABLE_FBDEV=ON -DENABLE_EGL=ON -DENABLE_X11=OFF -DENABLE_NOGUI=ON -DUSE_DISCORD_PRESENCE=OFF -DCMAKE_BUILD_TYPE=Release"

makeinstall_target() {
export CXXFLAGS="`echo $CXXFLAGS | sed -e "s|-O.|-O2|g"`"
mkdir -p $INSTALL/usr/config/emuelec/bin
cp -rf $PKG_BUILD/.${TARGET_NAME}/Binaries/dolphin-emu-nogui $INSTALL/usr/config/emuelec/bin
cp -rf $PKG_DIR/scripts/* $INSTALL/usr/config/emuelec/bin

mkdir -p $INSTALL/usr/config/emuelec/configs/dolphin-emu
cp -rf $PKG_BUILD/Data/Sys/* $INSTALL/usr/config/emuelec/configs/dolphin-emu
cp -rf $PKG_DIR/config/* $INSTALL/usr/config/emuelec/configs/dolphin-emu
}
